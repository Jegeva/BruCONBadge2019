#include "nokialcd.h"
#include <freertos/task.h>

#define KB_STARTX       3
#define KB_STARTY       60
#define BLK_WIDTH       14
#define BLK_HEIGHT      14
#define CHAR_X_OFFSET   1
#define CHAR_Y_OFFSET   4
#define CHARSET_LEN     45
#define INPUT_MAX_LEN   16
#define ENT_NEG         4
#define ENT_BTN         44

const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_-'\" $%#";
char *input = NULL;
uint8_t curr_len = 0;
uint8_t done = 0;
uint8_t dirty = 1;
uint8_t curr_selected = 0;
char g_pins_levels[6];
TickType_t g_pins_debounce[6];
TickType_t delay = 50 / portTICK_PERIOD_MS;

static xQueueHandle g_gpio_evt_queue = NULL;

gpio_num_t g_pins_buttons[6] = { 
    PIN_BL,
    PIN_BR,
    PIN_BU,
    PIN_BD,
    PIN_BA,
    PIN_BB
};

static void draw_keyboard()
{
    char curr[2] = {0};
    int ctr = 0;

    //lcd_clearB12(B12_WHITE);

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 9; j++, ctr++) {
            lcd_setRect(KB_STARTX + (BLK_WIDTH * j),
                    KB_STARTY + (BLK_HEIGHT * i),
                    KB_STARTX + (BLK_WIDTH * j) + BLK_WIDTH,
                    KB_STARTY + (BLK_HEIGHT * i) + BLK_HEIGHT,
                    1,
                    B12_WHITE);
            
            lcd_setRect(KB_STARTX + (BLK_WIDTH * j),
                    KB_STARTY + (BLK_HEIGHT * i),
                    KB_STARTX + (BLK_WIDTH * j) + BLK_WIDTH,
                    KB_STARTY + (BLK_HEIGHT * i) + BLK_HEIGHT,
                    0,
                    B12_BLACK);

            curr[0] = chars[ctr];
            lcd_setStr((char *)&curr,
                    KB_STARTY + (CHAR_X_OFFSET + BLK_WIDTH * i),
                    KB_STARTX + (CHAR_Y_OFFSET + BLK_HEIGHT * j),
                    B12_BLACK,
                    B12_WHITE,
                    0,
                    0);
    
            if (j == 8 && i == 4) {
                lcd_setStr("Ent",
                        KB_STARTY + (CHAR_X_OFFSET + BLK_WIDTH * i),
                        KB_STARTX + (CHAR_Y_OFFSET + BLK_HEIGHT * j) - ENT_NEG,
                        B12_BLACK,
                        B12_WHITE,
                        0,
                        0);
            }
        }
    }

    lcd_setRect(KB_STARTX,
            KB_STARTY,
            KB_STARTX + BLK_WIDTH,
            KB_STARTY + BLK_HEIGHT,
            1,
            B12_RED);
    
    lcd_setRect(KB_STARTX,
            KB_STARTY,
            KB_STARTX + BLK_WIDTH,
            KB_STARTY + BLK_HEIGHT,
            0,
            B12_BLACK);

    curr[0] = chars[0];
    lcd_setStr((char *)&curr,
            KB_STARTY + CHAR_X_OFFSET,
            KB_STARTX + CHAR_Y_OFFSET,
            B12_WHITE,
            B12_RED,
            0,
            0);
}

static void draw_selection(uint8_t old, uint8_t new)
{
    char chr[2] = {0};
    uint8_t old_x = old % 9;
    uint8_t old_y = old / 9;
    uint8_t new_x = new % 9;
    uint8_t new_y = new / 9;
   
    // old 
    lcd_setRect(KB_STARTX + (BLK_WIDTH * old_x),
            KB_STARTY + (BLK_HEIGHT * old_y),
            KB_STARTX + (BLK_WIDTH * old_x) + BLK_WIDTH,
            KB_STARTY + (BLK_HEIGHT * old_y) + BLK_HEIGHT,
            1,
            B12_WHITE);
    
    lcd_setRect(KB_STARTX + (BLK_WIDTH * old_x),
            KB_STARTY + (BLK_HEIGHT * old_y),
            KB_STARTX + (BLK_WIDTH * old_x) + BLK_WIDTH,
            KB_STARTY + (BLK_HEIGHT * old_y) + BLK_HEIGHT,
            0,
            B12_BLACK);

    if (old != ENT_BTN) {
        chr[0] = chars[old];
        lcd_setStr((char *)&chr,
                KB_STARTY + (CHAR_X_OFFSET + BLK_WIDTH * old_y),
                KB_STARTX + (CHAR_Y_OFFSET + BLK_HEIGHT * old_x),
                B12_BLACK,
                B12_WHITE,
                0,
                0);
    } else {
        lcd_setStr("Ent",
                KB_STARTY + (CHAR_X_OFFSET + BLK_WIDTH * old_y),
                KB_STARTX + (CHAR_Y_OFFSET + BLK_HEIGHT * old_x) - ENT_NEG,
                B12_BLACK,
                B12_WHITE,
                0,
                0);
    }
   
    // new 
    lcd_setRect(KB_STARTX + (BLK_WIDTH * new_x),
            KB_STARTY + (BLK_HEIGHT * new_y),
            KB_STARTX + (BLK_WIDTH * new_x) + BLK_WIDTH,
            KB_STARTY + (BLK_HEIGHT * new_y) + BLK_HEIGHT,
            1,
            B12_RED);
    
    lcd_setRect(KB_STARTX + (BLK_WIDTH * new_x),
            KB_STARTY + (BLK_HEIGHT * new_y),
            KB_STARTX + (BLK_WIDTH * new_x) + BLK_WIDTH,
            KB_STARTY + (BLK_HEIGHT * new_y) + BLK_HEIGHT,
            0,
            B12_BLACK);

    if (new != ENT_BTN) {
        chr[0] = chars[new];
        lcd_setStr((char *)&chr,
                KB_STARTY + (CHAR_X_OFFSET + BLK_WIDTH * new_y),
                KB_STARTX + (CHAR_Y_OFFSET + BLK_HEIGHT * new_x),
                B12_WHITE,
                B12_RED,
                0,
                0);
    } else {
        lcd_setStr("Ent",
                KB_STARTY + (CHAR_X_OFFSET + BLK_WIDTH * new_y),
                KB_STARTX + (CHAR_Y_OFFSET + BLK_HEIGHT * new_x) - ENT_NEG,
                B12_WHITE,
                B12_RED,
                0,
                0);
    }
    
}

static void btn_click_handler(uint32_t io_num, uint32_t level)
{
    uint8_t old_selected = curr_selected;
    
    switchbacklight(1);

    switch(io_num) {
    case PIN_BL:
        if (curr_selected) {
            curr_selected -= 1;
            dirty = 1;
        }
        break;
    case PIN_BR:
        if (curr_selected < CHARSET_LEN - 1) {
            curr_selected += 1;
            dirty = 1;
        }
        break;
    case PIN_BU:
        if (curr_selected > 8) {
            curr_selected -= 9;
            dirty = 1;
        }
        break;
    case PIN_BD:
        if (curr_selected < (CHARSET_LEN - 9)) {
            curr_selected += 9;
            dirty = 1;
        }
        break;
    case PIN_BA:
        if (curr_len) {
            input[curr_len] = '\0';
            curr_len--;
            dirty = 1;
        }
        break;
    case PIN_BB:
        if (curr_selected == ENT_BTN && curr_len) {
            done = 1;
        } else if (curr_len < INPUT_MAX_LEN) {
            input[curr_len] = chars[curr_selected];
            curr_len++;
            dirty = 1;
        }
        break;
    }

    if (dirty) {
        draw_selection(old_selected, curr_selected);    
        dirty = 0;
        lcd_sync();
    }
}

static void gpio_task()
{
    uint32_t io_num;
    int pinrank;
    char level;
    char prev_level;
    
    for(;;) {
        if(xQueueReceive(g_gpio_evt_queue, &io_num, portMAX_DELAY)) {
            level = gpio_get_level(io_num);
            switch(io_num){
            case PIN_BL:
                pinrank=0;
                break;
            case PIN_BR:
                pinrank=1;
                break;
            case PIN_BU:
                pinrank=2;
                break;
            case PIN_BD:
                pinrank=3;
                break;
            case PIN_BA:
                pinrank=4;
                break;
            case PIN_BB:
                pinrank=5;
                break;
            default:
                return;
                break;
            }
            
            prev_level = g_pins_levels[pinrank];
            g_pins_levels[pinrank] = level;
            
            if((xTaskGetTickCount() - g_pins_debounce[pinrank]) > 0){
                g_pins_debounce[pinrank] = xTaskGetTickCount();
            } else {
                prev_level = 0;
            }

            if(prev_level && (level == 0))
                btn_click_handler(io_num,level);
        }
    }
}


static void IRAM_ATTR handler_gpio_buttons(void *args) 
{
  uint32_t gpio_num = (uint32_t) args;
  xQueueSendFromISR(g_gpio_evt_queue, &gpio_num, NULL);
}

static void task_init(TaskHandle_t *task_handle)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_PIN_INTR_ANYEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = 0; // all are physically pulled up
    io_conf.pin_bit_mask =
        (1ULL<<PIN_BR ) |
        (1ULL<<PIN_BU ) |
        (1ULL<<PIN_BD ) |
        (1ULL<<PIN_BA ) |
        (1ULL<<PIN_BB ) |
        (1ULL<<PIN_BL );

    g_gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, task_handle);
    gpio_config(&io_conf);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(PIN_BL, handler_gpio_buttons, (void*)PIN_BL);
    gpio_isr_handler_add(PIN_BR, handler_gpio_buttons, (void*)PIN_BR);
    gpio_isr_handler_add(PIN_BU, handler_gpio_buttons, (void*)PIN_BU);
    gpio_isr_handler_add(PIN_BD, handler_gpio_buttons, (void*)PIN_BD);
    gpio_isr_handler_add(PIN_BB, handler_gpio_buttons, (void*)PIN_BB);
    gpio_isr_handler_add(PIN_BA, handler_gpio_buttons, (void*)PIN_BA);
}

char *get_keyboard_input()
{
    TaskHandle_t task_handle = NULL;
    input = calloc(16, sizeof(char));

    switchbacklight(1);

    if (!input)
        return NULL;

    draw_keyboard();
    lcd_sync();
    task_init(&task_handle);

    while (!done)
        vTaskDelay(delay);

    vTaskDelete(task_handle);
    vQueueDelete(g_gpio_evt_queue);
    gpio_uninstall_isr_service(0);

    return input;
}
