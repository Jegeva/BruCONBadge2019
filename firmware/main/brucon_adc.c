#include "brucon_adc.h"
#include "brucon_nvs.h"
static esp_adc_cal_characteristics_t *adc_chars;

static const adc_atten_t atten = ADC_ATTEN_DB_11;
static const adc_unit_t unit = ADC_UNIT_1;
#define DEFAULT_VREF 1100
adc1_channel_t chan_bat =ADC1_CHANNEL_4;//gpio 32
static esp_adc_cal_characteristics_t *adc_chars;

#define NO_OF_SAMPLES 200
char adcs_setup=0;
void setup_adcs(){
  adc_unit_t unit = ADC_UNIT_1;

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(chan_bat, atten);
  adc_chars = calloc(1, sizeof(esp_adc_cal_characteristics_t));
  esp_adc_cal_value_t val_type = esp_adc_cal_characterize(unit, atten, ADC_WIDTH_BIT_12, DEFAULT_VREF, adc_chars);
 
  if (val_type == ESP_ADC_CAL_VAL_EFUSE_VREF) {
    printf("eFuse Vref");
  } else if (val_type == ESP_ADC_CAL_VAL_EFUSE_TP) {
    printf("Two Point");
  } else {
    printf("Default");
  }
  adcs_setup = 1;

}

volatile char CalBat=0;
volatile char getBat=0;
volatile uint16_t Bat_level=0;

void calBatTask(){
if(!adcs_setup)
  setup_adcs();
 while(1){
   vTaskDelay(1000);
 }
}

void getBatTask(){
if(!adcs_setup)
  setup_adcs();
 getBat=1;
 uint32_t adc_reading = 0;
 //Multisampling
 for (int i = 0; i < NO_OF_SAMPLES; i++) {
   if (unit == ADC_UNIT_1) {
     adc_reading += adc1_get_raw((adc1_channel_t)chan_bat);
   }
 }
 adc_reading /= NO_OF_SAMPLES;
 //Convert adc_reading to voltage in mV
 uint32_t voltage = esp_adc_cal_raw_to_voltage(adc_reading, adc_chars);
 printf("BAT Raw: %d\tVoltage: %dmV\n", adc_reading, voltage);
 Bat_level=adc_reading;
 getBat=0;
  while(1){ vTaskDelay(1000);};
}
