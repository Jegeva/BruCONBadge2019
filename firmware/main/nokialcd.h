#ifndef __NOKIA_LCD_H
#define __NOKIA_LCD_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rom/ets_sys.h>
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "battery.h"
#include "math.h"

void MapWestL(void* arg);
void MapNovoL(void* arg);



#define LCD_SCK CONFIG_NOKIA6100_LCD_GPIO_SCK
#define LCD_DIO CONFIG_NOKIA6100_LCD_GPIO_DIO
#define LCD_RST CONFIG_NOKIA6100_LCD_GPIO_RST
#define LCD_CS  CONFIG_NOKIA6100_LCD_GPIO_CS

#define ENSCR_PIN CONFIG_NOKIA6100_LCD_GPIO_BACKLIGHT


#define PIN_BL CONFIG_CROSS_PIN_BUTTON_LEFT
#define PIN_BR CONFIG_CROSS_PIN_BUTTON_RIGHT
#define PIN_BU CONFIG_CROSS_PIN_BUTTON_UP
#define PIN_BD CONFIG_CROSS_PIN_BUTTON_DOWN
#define PIN_BA CONFIG_CROSS_PIN_BUTTON_A
#define PIN_BB CONFIG_CROSS_PIN_BUTTON_B

#define PIN_NCHARGING 34

#define LCD_SCK_L gpio_set_level(LCD_SCK,0)
#define LCD_SCK_H gpio_set_level(LCD_SCK,1)
#define LCD_DIO_L gpio_set_level(LCD_DIO,0)
#define LCD_DIO_H gpio_set_level(LCD_DIO,1)
#define LCD_RST_L gpio_set_level(LCD_RST,0)
#define LCD_RST_H gpio_set_level(LCD_RST,1)
#define LCD_CS_L  gpio_set_level(LCD_CS ,0)
#define LCD_CS_H  gpio_set_level(LCD_CS ,1)

#define dl_us(x)  ets_delay_us(x)
#define clockit   LCD_SCK_L;                    \
    dl_us(1);                                   \
    LCD_SCK_H;

#define EPSON 1
#define PHILLIPS 0

#define LCD_COMMAND 0
#define LCD_DATA 1

// ripped from the arduino library

#define ROW_LENGTH	132
#define COL_HEIGHT	132

//********************************************************************
//					EPSON Controller Definitions
//********************************************************************
#define DISON       0xAF	// Display on
#define DISOFF      0xAE	// Display off
#define DISNOR      0xA6	// Normal display
#define DISINV      0xA7	// Inverse display
#define SLPIN       0x95	// Sleep in
#define SLPOUT      0x94	// Sleep out
#define COMSCN      0xBB	// Common scan direction
#define DISCTL      0xCA	// Display control
#define PASET       0x75	// Page address set
#define CASET       0x15	// Column address set
#define DATCTL      0xBC	// Data scan direction, etc.
#define RGBSET8     0xCE	// 256-color position set
#define RAMWR       0x5C	// Writing to memory
#define RAMRD       0x5D	// Reading from memory
#define PTLIN       0xA8	// Partial display in
#define PTLOUT      0xA9	// Partial display out
#define RMWIN       0xE0	// Read and modify write
#define RMWOUT      0xEE	// End
#define ASCSET      0xAA	// Area scroll set
#define SCSTART     0xAB	// Scroll start set
#define OSCON       0xD1	// Internal oscillation on
#define OSCOFF      0xD2	// Internal osciallation off
#define PWRCTR      0x20	// Power control
#define VOLCTR      0x81	// Electronic volume control
#define VOLUP       0xD6	// Increment electronic control by 1
#define VOLDOWN     0xD7	// Decrement electronic control by 1
#define TMPGRD      0x82	// Temperature gradient set
#define EPCTIN      0xCD	// Control EEPROM
#define EPCOUT      0xCC	// Cancel EEPROM control
#define EPMWR       0xFC	// Write into EEPROM
#define EPMRD       0xFD	// Read from EEPROM
#define EPSRRD1     0x7C	// Read register 1
#define EPSRRD2     0x7D	// Read register 2
#define NOP         0x25	// No op

//********************************************************************
//			PHILLIPS Controller Definitions
//********************************************************************
//LCD Commands
#define	NOPP		0x00	// No operation
#define	BSTRON		0x03	// Booster voltage on
#define SLEEPIN     0x10	// Sleep in
#define	SLEEPOUT	0x11	// Sleep out
#define	NORON		0x13	// Normal display mode on
#define	INVOFF		0x20	// Display inversion off
#define INVON      	0x21	// Display inversion on
#define	SETCON		0x25	// Set contrast
#define DISPOFF     0x28	// Display off
#define DISPON      0x29	// Display on
#define CASETP      0x2A	// Column address set
#define PASETP      0x2B	// Page address set
#define RAMWRP      0x2C	// Memory write
#define RGBSET	    0x2D	// Color set
#define	MADCTL		0x36	// Memory data access control
#define	PCOLMOD		0x3A	// Interface pixel format
#define DISCTR      0xB9	// Super frame inversion
#define	EC			0xC0	// Internal or external oscillator

//*******************************************************
//				12-Bit Color Definitions
//*******************************************************
#define B12_BLACK	    0xF000
#define B12_NAVY 		0xF008
#define B12_BLUE		0xF00F
#define B12_TEAL 		0xF088
#define B12_EMERALD		0xF0C5
#define	B12_GREEN		0xF0F0
#define B12_CYAN		0xF0FF
#define B12_SLATE 		0xF244
#define B12_INDIGO  	0xF408
#define B12_TURQUOISE	0xF4ED
#define B12_OLIVE 		0xF682
#define B12_MAROON 		0xF800
#define B12_PURPLE 		0xF808
#define B12_GRAY 		0xF888
#define B12_SKYBLUE		0xF8CE
#define B12_BROWN		0xFB22
#define B12_CRIMSON 	0xFD13
#define B12_ORCHID 		0xFD7D
#define B12_RED			0xFF00
#define B12_MAGENTA		0xFF0F
#define B12_ORANGE 		0xFF40
#define B12_PINK		0xFF6A
#define B12_CORAL 		0xFF75
#define B12_SALMON 		0xFF87
#define B12_GOLD 		0xFFD0
#define B12_YELLOW		0xFFF0
#define B12_WHITE		0xFFFF

//*******************************************************
//                              Circle Definitions
//*******************************************************
#define FULLCIRCLE 1
#define OPENSOUTH  2
#define OPENNORTH  3
#define OPENEAST   4
#define OPENWEST   5
#define OPENNORTHEAST 6
#define OPENNORTHWEST 7
#define OPENSOUTHEAST 8
#define OPENSOUTHWEST 9
#define FILL 0
#define ESE 1
#define ENE 2
#define WSW 3
#define WNW 4
#define SSE 5
#define NNE 6
#define SSW 7
#define NNW 8

#define ROW_LENGTH	132
#define COL_HEIGHT	132

void init_lcd(int type);
void switchbacklight(int state);
void lcd_clearB12(uint16_t color);

void lcd_contrast(char setting);
void lcd_setArc(int x0, int y0, int radius, int arcSegments[], int numSegments, int lineThickness, int color);
void lcd_setCircle (int x0, int y0, int radius, int color, int lineThickness);
void lcd_setChar(char c, int x, int y, int fColor, int bColor, char uselastfill);
void lcd_setStr(char *pString, int x, int y, int fColor, int bColor, char uselastfill, char newline);
void lcd_setLine(int x0, int y0, int x1, int y1, int color);
void lcd_setRect(int x0, int y0, int x1, int y1, unsigned char fill, int color);
void lcd_printLogo(void);
void lcd_printBMP(char * image_main); //prints an image (BMP);
void lcd_off(void);
void lcd_on(void);
void lcd_sync(void);
void bruconlogo();

typedef uint16_t pixel_t;

typedef struct image_s {
    int w;
    int h;
    pixel_t buffer[];
} image_t;

void draw_pixel(int x, int y, pixel_t color, unsigned int blend);
void draw_image(const image_t * img, int x, int y);


typedef struct {
    const uint16_t * data;
    const uint16_t * lookup;
    const uint16_t height;
    const uint16_t min;
    const uint16_t max;
} font_t;

typedef struct {
    uint16_t h;
    uint16_t w;
    int16_t left;
    int16_t top;
    int16_t advance;
    uint16_t buffer[];
} font_char_t;

int string_width(const font_t * font, const char * str);
void draw_string(const font_t * font, int x, int y, pixel_t color, const char * str);

#define LEFT     0x00000001
#define CENTER   0x00000002
#define RIGHT    0x00000003
#define ALIGN_H  0x0000000F

#define TOP      0x00000010
#define VCENTER  0x00000020
#define BOTTOM   0x00000030
#define ALIGN_V  0x000000F0

#define WRAP     0x00000100

void draw_string_aligned(const font_t * font, int x, int y, int w, int h,
        pixel_t color, unsigned int flags, const char *str);

extern volatile char preventbacklighttimeoutTask ;

#endif

// vim: expandtab:ts=4:sw=4
