#ifndef _TFT_H_
#define _TFT_H_

#include "stdint.h"
#include "stdio.h"
#include "SPI.h"
#include "logo.h"
#include "font.h"
#include "spi_flash.h"
#include "include.h"


#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t


//#define ICACHE_FLASH_ATTR __attribute__((section(".irom0.text")))
//#define ICACHE_RODATA_ATTR __attribute__((section(".irom.text"))) 

void ICACHE_FLASH_ATTR tft_write_cmd(unsigned char cmd);
void ICACHE_FLASH_ATTR tft_write_dat(unsigned char dat);
void ICACHE_FLASH_ATTR tft_set_window(u16 x1, u16 y1, u16 x2, u16 y2);
void ICACHE_FLASH_ATTR tft_clear(u8 Color);
void ICACHE_FLASH_ATTR tft_clear_color(u32 Color);
void ICACHE_FLASH_ATTR tft_show_frame1(int index);
void ICACHE_FLASH_ATTR tft_show_frame(void);
void ICACHE_FLASH_ATTR lcd_draw_point(u16 x, u16 y, u16 color);
void ICACHE_FLASH_ATTR tft_show_logo(void);
void ICACHE_FLASH_ATTR tft_drawLine(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
void ICACHE_FLASH_ATTR tft_init(void);
void ICACHE_FLASH_ATTR lcd_show_char(u16 x, u16 y, u8 size, u8 chrx, u16 color);
void ICACHE_FLASH_ATTR lcd_show_string(u16 x, u16 y, u8 size, char *str, u16 color);
void ICACHE_FLASH_ATTR lcd_show_num(u16 x, u16 y, u8 size, int num, u16 color);
void ICACHE_FLASH_ATTR lcd_printf(u16 x, u16 y, u16 size, u16 color, char *fmt, ...);
void ICACHE_FLASH_ATTR tft_show_bmp(u16 x1, u16 y1, u8 size, u8 *BMP);
/*
void tftfonthz(char *code, unsigned char *mat, u8 size);
void Show_Font(u16 x, u16 y, char *font, u8 size, u16 color);




void tftdis_icon(u16 x, u16 y, u16 size, u32 addr);
*/

#endif