#ifndef __SETTINGS_H
#define __SETTINGS_H
#include "esp_system.h"
#include "nvs_flash.h"
#include "nvs.h"

#define WIFI_PASSWORD  ""
#define WIFI_SSID      ""


const char * get_wifi_netname();
const char * get_wifi_pass();


#endif
