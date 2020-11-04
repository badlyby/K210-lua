/* Copyright 2018 Canaan Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef _LCD_H_
#define _LCD_H_

#include <stdint.h>
#include "st7789.h"

/* clang-format off */
#define BLACK       0x0000
#define NAVY        0x000F
#define DARKGREEN   0x03E0
#define DARKCYAN    0x03EF
#define MAROON      0x7800
#define PURPLE      0x780F
#define OLIVE       0x7BE0
#define LIGHTGREY   0xC618
#define DARKGREY    0x7BEF
#define BLUE        0x001F
#define GREEN       0x07E0
#define CYAN        0x07FF
#define RED         0xF800
#define MAGENTA     0xF81F
#define YELLOW      0xFFE0
#define WHITE       0xFFFF
#define ORANGE      0xFD20
#define GREENYELLOW 0xAFE5
#define PINK        0xF81F
#define USER_COLOR  0xAA55
/* clang-format on */

typedef enum _lcd_dir
{
    DIR_XY_RLUD = 0x00,
    DIR_YX_RLUD = 0x20,
    DIR_XY_LRUD = 0x40,
    DIR_YX_LRUD = 0x60,
    DIR_XY_RLDU = 0x80,
    DIR_YX_RLDU = 0xA0,
    DIR_XY_LRDU = 0xC0,
    DIR_YX_LRDU = 0xE0,
    DIR_XY_MASK = 0x20,
    DIR_MASK = 0xE0,
    DIR_RGB2BRG = 0x08, // lcd_set_direction(DIR_YX_RLUD | DIR_RGB2BRG); // 0x28
} lcd_dir_t;

typedef struct _lcd_ctl
{
    uint8_t mode;
    uint8_t dir;
    uint16_t width;
    uint16_t height;
    uint16_t start_offset_w0;
    uint16_t start_offset_h0;
    uint16_t start_offset_w1;
    uint16_t start_offset_h1;
    uint16_t start_offset_w;
    uint16_t start_offset_h;
} lcd_ctl_t;

void lcd_polling_enable(void);
void lcd_interrupt_enable(void);
int lcd_init(uint32_t freq, bool oct, uint16_t offset_w, uint16_t offset_h, uint16_t offset_w1, uint16_t offset_h1, bool invert_color, uint8_t dir, uint16_t width, uint16_t height);
void lcd_destroy();
void lcd_clear(uint16_t color);
void lcd_set_freq(uint32_t freq);
uint32_t lcd_get_freq();
void lcd_set_direction(lcd_dir_t dir);
void lcd_set_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color);
void lcd_draw_string(uint16_t x, uint16_t y, char *str, uint16_t color);
void lcd_draw_buf(uint16_t x1, uint16_t y1, uint16_t width, uint16_t height, const uint32_t *ptr);
void lcd_fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, uint16_t color);
void lcd_set_offset(uint16_t offset_w, uint16_t offset_h);

uint16_t lcd_get_width();
uint16_t lcd_get_height();


typedef void (*lcd_preinit_handler_t)(void);
void lcd_preinit_register_handler(lcd_preinit_handler_t handler);
void lcd_init_sequence_for_ili9486(void);

#endif

