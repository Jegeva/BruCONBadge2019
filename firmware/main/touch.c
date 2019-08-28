#include "touch.h"

volatile bool s_pad_activated[6];
uint32_t s_pad_init_val[6];

#define RANK_BL 0
#define RANK_BR 1
#define RANK_BU 2
#define RANK_BD 3
#define RANK_BA 4
#define RANK_BB 5

#define ISTOUCHPIN(p) \
    ( p == 33 || p == 32 || p == 27 || p == 14 || p == 12 || p == 13 || p == 15 || p == 2 || p == 0 || p == 4 )
#define ISTOUCHPAD(p)\
    (p>= 0 && p <TOUCH_PAD_MAX)

inline int pinfortouchpad(int p)
{
    assert(ISTOUCHPAD(p));
    
    switch(p){
    case 9:
        return 32;
    case 8:
        return 33;
    case 7:
        return 27;
    case 6:
        return 14;
    case 5:
        return 12;
    case 4:
        return 13;
    case 3:
        return 15;
    case 2:
        return 2;
    case 1:
        return 0;
    case 0:
        return 4;
    }
    return 0;    
}

inline int touchpadforpin(int p)
{
    assert( ISTOUCHPIN(p) );
    
    switch(p){
    case 33:
        return 8;
    case 32:
        return 9;
    case 27:
        return 7;
    case 14:
        return 6;
    case 12:
        return 5;
    case 13:
        return 4;
    case 15:
        return 3;
    case 2:
        return 2;
    case 0:
        return 1;
    case 4:
        return 0;
    }
    return 0;    
}

uint8_t pad_BL;
uint8_t pad_BR;
uint8_t pad_BU;
uint8_t pad_BD;
uint8_t pad_BA;
uint8_t pad_BB;
xQueueHandle gpio_evt_queue = NULL;

static void IRAM_ATTR handler_touch_buttons(void *args) {
  uint32_t pad_intr = touch_pad_get_status();
  uint32_t gpio_num;
//  printf("isr touch: %d\n",pad_intr);
  
  for (int i = 0; i < TOUCH_PAD_MAX; i++) {
      if ((pad_intr >> i) & 0x01) {
          gpio_num = pinfortouchpad(i);
          xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
      }
  } 
}

void init_touch()
{
    printf("init touch\n");
    touch_pad_init();
    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    pad_BL=touchpadforpin(PIN_BL);
    pad_BR=touchpadforpin(PIN_BR);
    pad_BU=touchpadforpin(PIN_BU);
    pad_BD=touchpadforpin(PIN_BD);
    pad_BA=touchpadforpin(PIN_BA);
    pad_BB=touchpadforpin(PIN_BB);

   

    uint16_t touch_value;
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    touch_pad_config(pad_BL, TOUCH_THRESH_NO_USE);
    touch_pad_config(pad_BR, TOUCH_THRESH_NO_USE);
    touch_pad_config(pad_BU, TOUCH_THRESH_NO_USE);
    touch_pad_config(pad_BD, TOUCH_THRESH_NO_USE);
    touch_pad_config(pad_BA, TOUCH_THRESH_NO_USE);
    touch_pad_config(pad_BB, TOUCH_THRESH_NO_USE);
    
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);

     touch_pad_read_filtered(pad_BL, &touch_value);
    s_pad_init_val[RANK_BL]=touch_value;
    touch_pad_set_thresh(pad_BL, touch_value * 2 / 3);
    touch_pad_read_filtered(pad_BR, &touch_value);
    s_pad_init_val[RANK_BR]=touch_value;
    touch_pad_set_thresh(pad_BR, touch_value * 2 / 3);
    touch_pad_read_filtered(pad_BU, &touch_value);
    s_pad_init_val[RANK_BU]=touch_value;
    touch_pad_set_thresh(pad_BU, touch_value * 2 / 3);
    touch_pad_read_filtered(pad_BD, &touch_value);
    s_pad_init_val[RANK_BD]=touch_value;
    touch_pad_set_thresh(pad_BD, touch_value * 2 / 3);
    touch_pad_read_filtered(pad_BA, &touch_value);
    s_pad_init_val[RANK_BA]=touch_value;
    touch_pad_set_thresh(pad_BA, touch_value * 2 / 3);
    touch_pad_read_filtered(pad_BB, &touch_value);
    s_pad_init_val[RANK_BB]=touch_value;
    touch_pad_set_thresh(pad_BB, touch_value * 2 / 3);

    /*
    for(int i = 0; i < 50; i++){

        uint32_t pad_intr = touch_pad_get_status();
        touch_pad_clear_status();
        touch_pad_read_filtered(pad_BL, &touch_value);
        s_pad_init_val[RANK_BL]=touch_value;
        touch_pad_read_filtered(pad_BR, &touch_value);
        s_pad_init_val[RANK_BR]=touch_value;
        touch_pad_read_filtered(pad_BU, &touch_value);
        s_pad_init_val[RANK_BU]=touch_value;
        touch_pad_read_filtered(pad_BD, &touch_value);
        s_pad_init_val[RANK_BD]=touch_value;
        touch_pad_read_filtered(pad_BA, &touch_value);
        s_pad_init_val[RANK_BA]=touch_value;
        touch_pad_read_filtered(pad_BB, &touch_value);
        s_pad_init_val[RANK_BB]=touch_value;
        printf("\n%d\n",pad_intr);
        
        printf("BL %d %d init %d %d \n",PIN_BL,touchpadforpin(PIN_BL),s_pad_init_val[RANK_BL],(pad_intr & (1<<touchpadforpin(PIN_BL)))>0);
        printf("BR %d %d init %d %d \n",PIN_BR,touchpadforpin(PIN_BR),s_pad_init_val[RANK_BR],(pad_intr & (1<<touchpadforpin(PIN_BR)))>0);
        printf("BU %d %d init %d %d \n",PIN_BU,touchpadforpin(PIN_BU),s_pad_init_val[RANK_BU],(pad_intr & (1<<touchpadforpin(PIN_BU)))>0);    
        printf("BD %d %d init %d %d \n",PIN_BD,touchpadforpin(PIN_BD),s_pad_init_val[RANK_BD],(pad_intr & (1<<touchpadforpin(PIN_BD)))>0);
        printf("BA %d %d init %d %d \n",PIN_BA,touchpadforpin(PIN_BA),s_pad_init_val[RANK_BA],(pad_intr & (1<<touchpadforpin(PIN_BA)))>0);    
        printf("BB %d %d init %d %d \n",PIN_BB,touchpadforpin(PIN_BB),s_pad_init_val[RANK_BB],(pad_intr & (1<<touchpadforpin(PIN_BB)))>0);
        vTaskDelay(100);
    }
    
    */

    touch_pad_intr_enable();
    touch_pad_clear_status();
    touch_pad_isr_register(handler_touch_buttons, NULL);
    printf("init touch done\n");
}
