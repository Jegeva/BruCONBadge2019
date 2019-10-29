#ifndef __MENU_H
#define __MENU_H
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_system.h>
#include "lwip/inet.h"
#include "lwip/ip4_addr.h"
#include "lwip/dns.h"
#include "url.h"
#include "cJSON.h"
#include "battery.h"
void display_menu(uint8_t clicked_gui_value);

typedef struct menuIt * menuItp;
typedef struct menuIt
{
    char * menuText;
    uint8_t rank;
    char * schedentry;
    char * schedloc;
    char * schedspk;
    char schedtype;
    menuItp child;
    menuItp parent;
    menuItp prev;
    menuItp next;
    void (* funcp)(void*);
} menuItem;

extern menuItp menuhead;
extern menuItp schedhead_d1;
extern menuItp schedhead_d2;
extern menuItp schedhead_d3;
extern TickType_t last_click;
extern volatile char batt_update;
extern char * netshed;
extern xQueueHandle gpio_evt_queue ;
extern menuItem * currMenuItem;
extern menuItem fakeplaceholder;



extern void (*volatile menu_mgt_func)(uint32_t,uint32_t);

menuItem * changemenuItem(menuItem *);
void set_status_bar_text(char * b);
void defaultmenu();
void manage_click(uint32_t value,uint32_t level );
void manage_click_menu_orig(uint32_t value,uint32_t level );
void manage_click_menu(uint32_t value,uint32_t level );
void display_taskbar();
extern char notaskbar;



#endif
