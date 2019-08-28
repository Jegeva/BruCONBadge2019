#ifndef BRUCON_TOUCH_H
#define BRUCON_TOUCH_H

#include "menu.h"
#include "nokialcd.h"
#include "driver/touch_pad.h"

#define TOUCH_PAD_NO_CHANGE   (-1)
#define TOUCH_THRESH_NO_USE   (0)
#define TOUCH_FILTER_MODE_EN  (1)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

extern xQueueHandle gpio_evt_queue;

void init_touch();



#endif
