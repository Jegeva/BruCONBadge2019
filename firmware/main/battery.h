#ifndef BRUCON_BATTERY_H
#define BRUCON_BATTERY_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rom/ets_sys.h>
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "expat.h"
#include <freertos/queue.h>
#include <esp_event.h>
#include <esp_event_loop.h>
#include <sdkconfig.h>
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "menu.h"
// usb plugged
#define POWSTAT_USB   1

// charged
#define POWSTAT_STAT1 2

// charging
#define POWSTAT_STAT2 4


typedef struct
{

    uint8_t powstat;
    uint8_t percent;
    uint32_t vbat;


} batteryinfo_t;

extern volatile char preventbacklighttimeoutTask ;
extern batteryinfo_t batteryinfo;
void     update_batteryinfo();

uint8_t  bat_pc(); // return percentage 0-100
void stop_bat_task();
void start_bat_task();

void enable_bat_interrupts();

extern xQueueHandle bat_evt_queue;

#endif
