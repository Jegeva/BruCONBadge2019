#include "battery.h"

uint8_t bat_inited = 0;
batteryinfo_t batteryinfo;
xQueueHandle bat_evt_queue=NULL;




uint8_t enabled_interrupts = 0;
volatile char batt_update = 1;
void enable_bat_interrupts()
{
    enabled_interrupts =1;

}


static void IRAM_ATTR handler_gpio_bat(void *args) {

    uint32_t gpio_num = (uint32_t) args;
    uint32_t level = gpio_get_level(gpio_num);
    uint32_t battpin = 9999;

    if(level){
        switch(gpio_num){
        case CONFIG_USB_GPIO_PIN:
            batteryinfo.powstat |= POWSTAT_USB;
        case CONFIG_STAT1_BAT_GPIO_PIN:
            batteryinfo.powstat |= POWSTAT_STAT1;
        case CONFIG_STAT2_BAT_GPIO_PIN:
            batteryinfo.powstat |= POWSTAT_STAT2;
        }
    } else {
        switch(gpio_num){
        case CONFIG_USB_GPIO_PIN:
            batteryinfo.powstat &= ~POWSTAT_USB;
        case CONFIG_STAT1_BAT_GPIO_PIN:
            batteryinfo.powstat &= ~POWSTAT_STAT1;
        case CONFIG_STAT2_BAT_GPIO_PIN:
            batteryinfo.powstat &= ~POWSTAT_STAT2;
        }
    }
    batt_update = 1;
    
    xQueueSendFromISR(gpio_evt_queue, &battpin, NULL);
}

static esp_adc_cal_characteristics_t *adc_chars;
#define NO_OF_SAMPLES 500
static const adc_unit_t unit = ADC_UNIT_1;
#define DEFAULT_VREF 1100

uint8_t bat_pc()
{
    // dirty but does the job
    // the non linearity of the adc reading and vcc changes fucks things up and the batt range isn't covering a good rangeregarding th adc range
    // 3.2v batt (completely flat) registes as 3.950 so here is a corrected curve, more or less, nothing to write home about but does the job
    // and it is even worse when in bad battery conditon and not chargin...
    uint8_t pc;
    if(batteryinfo.powstat & POWSTAT_USB){
        
    if(batteryinfo.vbat > 4100)
        pc=100;
    if(batteryinfo.vbat < 4090)
        pc=90;
    if(batteryinfo.vbat < 4080)
        pc=80;
    if(batteryinfo.vbat < 4060)
        pc=70;
    if(batteryinfo.vbat < 4040)
        pc=60;
    if(batteryinfo.vbat < 4020)
        pc=50;
    if(batteryinfo.vbat < 4000)
        pc=30;
    if(batteryinfo.vbat < 3980)
        pc=10;
    if(batteryinfo.vbat < 3970)
        pc=5;
    if(batteryinfo.vbat < 3950)
        pc=0;
    } else {
    if(batteryinfo.vbat > 4100)
        pc=100;
    if(batteryinfo.vbat < 4100)
        pc=90;
    if(batteryinfo.vbat < 4090)
        pc=80;
    if(batteryinfo.vbat < 4080)
        pc=70;
    if(batteryinfo.vbat < 4070)
        pc=60;
    if(batteryinfo.vbat < 4060)
        pc=50;
    if(batteryinfo.vbat < 4030)
        pc=30;
    if(batteryinfo.vbat < 4020)
        pc=10;
    if(batteryinfo.vbat < 4010)
        pc=5;
    if(batteryinfo.vbat < 4000)
        pc=0;
    }
    
    return pc;
}

char taskret=0;

uint32_t get_bat_voltage()
{
    uint32_t raw = 0;    
    for (int i = 0; i < NO_OF_SAMPLES; i++) {
        raw += adc1_get_raw(ADC1_CHANNEL_7);
    }
    raw/= NO_OF_SAMPLES;

    // falls short by 100mV on high side for some reason
    raw = (esp_adc_cal_raw_to_voltage(raw, adc_chars))  ;

    return raw ;
}

volatile char bat_task_runing = 0;
TaskHandle_t batteryTask;

void battery_lvl_task()
{
    uint32_t battpin = 9999;
    while(1){
        bat_task_runing = 1;
        
        batteryinfo.vbat    = get_bat_voltage();
        batteryinfo.percent = bat_pc();
        printf("battery stat : %d vbat : %d mV %d%%\n",  batteryinfo.powstat,batteryinfo.vbat,batteryinfo.percent);
        xQueueSendFromISR(gpio_evt_queue, &battpin, NULL);
        vTaskDelay(1000);
    }
}

void stop_bat_task()
{
    if(bat_task_runing)
        vTaskDelete(batteryTask);
    bat_task_runing = 0;
    
}

void start_bat_task()
{
    if(!bat_task_runing)
        xTaskCreatePinnedToCore( battery_lvl_task , "batt_level" , 8192, &taskret , 5| portPRIVILEGE_BIT , &batteryTask ,0);
}


void bat_init()
{
    gpio_config_t io_conf;
    bat_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    // pins w/ interrupts
    printf("battery service init\n");
    io_conf.intr_type = GPIO_PIN_INTR_ANYEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0;
    io_conf.pin_bit_mask = (
        (1ULL<< CONFIG_USB_GPIO_PIN) |
        (1ULL<< CONFIG_STAT1_BAT_GPIO_PIN) |
        (1ULL<< CONFIG_STAT2_BAT_GPIO_PIN)
        );
    gpio_config(&io_conf);
    if(gpio_get_level(CONFIG_USB_GPIO_PIN))
        batteryinfo.powstat |= POWSTAT_USB;
    if(gpio_get_level(CONFIG_STAT1_BAT_GPIO_PIN))
        batteryinfo.powstat |= POWSTAT_STAT1;
    if(gpio_get_level(CONFIG_STAT2_BAT_GPIO_PIN))
        batteryinfo.powstat |= POWSTAT_STAT2;

    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(CONFIG_USB_GPIO_PIN	         , handler_gpio_bat, (void*)CONFIG_USB_GPIO_PIN  );
    gpio_isr_handler_add(CONFIG_STAT1_BAT_GPIO_PIN       , handler_gpio_bat, (void*)CONFIG_STAT1_BAT_GPIO_PIN  );
    gpio_isr_handler_add(CONFIG_STAT2_BAT_GPIO_PIN       , handler_gpio_bat, (void*)CONFIG_STAT2_BAT_GPIO_PIN  );
    

    // adc
  
    adc1_config_width(ADC_WIDTH_BIT_9);
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);

    adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
    esp_adc_cal_value_t val_type = esp_adc_cal_characterize(ADC_UNIT_1, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_9, DEFAULT_VREF, adc_chars);
    
    if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
        printf("eFuse Vref");
    } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
        printf("Two Point");
    } else {
        printf("Default");
    }

    batteryinfo.vbat=get_bat_voltage();
    batteryinfo.percent = bat_pc();

    start_bat_task();
    

    printf("battery service inited stat : %d vbat : %d mV\n",  batteryinfo.powstat,batteryinfo.vbat);
    bat_inited = 1;


}



void update_batteryinfo()
{
    if(! bat_inited)
        bat_init();


}
