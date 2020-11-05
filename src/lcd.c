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
#include <string.h>
#include <unistd.h>
#include "stdlib.h"
#include "lcd.h"
#include "font.h"
#include "sleep.h"

#define SWAP_16(x) ((x>>8&0xff) | (x<<8))

static lcd_ctl_t lcd_ctl;

static uint16_t* g_lcd_display_buff = NULL;
static uint16_t g_lcd_w = 0;
static uint16_t g_lcd_h = 0;
static bool g_lcd_init = false;

void lcd_polling_enable(void)
{
    lcd_ctl.mode = 0;
}

void lcd_interrupt_enable(void)
{
    lcd_ctl.mode = 1;
}

lcd_preinit_handler_t lcd_preinit_handler = NULL;

/**
 * Register Pre-initialization handler for lcd
 */
void lcd_preinit_register_handler(lcd_preinit_handler_t handler)
{
    lcd_preinit_handler = handler;
}

void lcd_init_sequence_for_ili9486(void)
{
    uint8_t t[15];
    tft_write_command(0XF1); /* Unk */
    t[0] = (0x36);
    t[1] = (0x04);
    t[2] = (0x00);
    t[3] = (0x3C);
    t[4] = (0X0F);
    t[5] = (0x8F);
    tft_write_byte(t, 6);

    tft_write_command(0XF2); /* Unk */
    t[0] = (0x18);
    t[1] = (0xA3);
    t[2] = (0x12);
    t[3] = (0x02);
    t[4] = (0XB2);
    t[5] = (0x12);
    t[6] = (0xFF);
    t[7] = (0x10);
    t[8] = (0x00);
    tft_write_byte(t, 9);

    tft_write_command(0XF8); /* Unk */
    t[0] = (0x21);
    t[1] = (0x04);
    tft_write_byte(t, 2);

    tft_write_command(0XF9); /* Unk */
    t[0] = (0x00);
    t[1] = (0x08);
    tft_write_byte(t, 2);

    tft_write_command(0x36); /* Memory Access Control */
    t[0] = (0x28);
    tft_write_byte(t, 1);

    tft_write_command(0xB4); /* Display Inversion Control */
    t[0] = (0x00);
    tft_write_byte(t, 1);

    // tft_write_command(0xB6); /* Display Function Control */
    // t[0] = (0x02);
    // // t[1] = (0x22);
    // tft_write_byte(t, 1);

    tft_write_command(0xC1); /* Power Control 2 */
    t[0] = (0x41);
    tft_write_byte(t, 1);
    
    tft_write_command(0xC5); /* Vcom Control */
    t[0] = (0x00);
    t[1] = (0x18);
    tft_write_byte(t, 2);

    tft_write_command(0xE0); /* Positive Gamma Control */
    t[0] = (0x0F);
    t[1] = (0x1F);
    t[2] = (0x1C);
    t[3] = (0x0C);
    t[4] = (0x0F);
    t[5] = (0x08);
    t[6] = (0x48);
    t[7] = (0x98);
    t[8] = (0x37);
    t[9] = (0x0A);
    t[10] = (0x13);
    t[11] = (0x04);
    t[12] = (0x11);
    t[13] = (0x0D);
    t[14] = (0x00);
    tft_write_byte(t, 15);

    tft_write_command(0xE1); /* Negative Gamma Control */
    t[0] = (0x0F);
    t[1] = (0x32);
    t[2] = (0x2E);
    t[3] = (0x0B);
    t[4] = (0x0D);
    t[5] = (0x05);
    t[6] = (0x47);
    t[7] = (0x75);
    t[8] = (0x37);
    t[9] = (0x06);
    t[10] = (0x10);
    t[11] = (0x03);
    t[12] = (0x24);
    t[13] = (0x20);
    t[14] = (0x00);
    tft_write_byte(t, 15);

    tft_write_command(0x3A); /* Interface Pixel Format */
    t[0] = (0x55);
    tft_write_byte(t, 1);

}

int lcd_init(uint32_t freq, bool oct, uint16_t offset_w0, uint16_t offset_h0, uint16_t offset_w1, uint16_t offset_h1, bool invert_color, uint8_t dir, uint16_t width, uint16_t height)
{
    uint8_t data = 0;
    lcd_ctl.dir = dir;
    lcd_ctl.width = width, lcd_ctl.height = height;
    lcd_ctl.start_offset_w0 = offset_w0;
    lcd_ctl.start_offset_h0 = offset_h0;
    lcd_ctl.start_offset_w1 = offset_w1;
    lcd_ctl.start_offset_h1 = offset_h1;
    if(g_lcd_w != width || g_lcd_h != height)
    {
        if(g_lcd_display_buff)
        {
            free(g_lcd_display_buff);
        }
        g_lcd_display_buff = (uint16_t*)malloc(width*height*2);
        if(!g_lcd_display_buff)
            return 12; //ENOMEM
        g_lcd_w = width;
        g_lcd_h = height;
    }
    tft_hard_init(freq, oct);
    /*soft reset*/
    tft_write_command(SOFTWARE_RESET);
    msleep(50);
    if (lcd_preinit_handler != NULL)
    {
        lcd_preinit_handler();
    }

    /*exit sleep*/
    tft_write_command(SLEEP_OFF);
    msleep(120);
    /*pixel format*/
    tft_write_command(PIXEL_FORMAT_SET);
    data = 0x55;
    tft_write_byte(&data, 1);
    msleep(10);
    
    g_lcd_init = true;

    lcd_set_direction(DIR_YX_RLDU);
    if(invert_color)
    {
        tft_write_command(INVERSION_DISPALY_ON);
        msleep(10);
    }
    tft_write_command(NORMAL_DISPALY_ON);
    msleep(10);
    /*display on*/
    tft_write_command(DISPALY_ON);
    // msleep(100);
    lcd_polling_enable();
    return 0;
}

void lcd_destroy()
{
    if(g_lcd_display_buff)
    {
        free(g_lcd_display_buff);
        g_lcd_display_buff = NULL;
    }
    g_lcd_w = 0;
    g_lcd_h = 0;
}

uint16_t lcd_get_width()
{
    return g_lcd_w;
}

uint16_t lcd_get_height()
{
    return g_lcd_h;
}

void lcd_set_direction(lcd_dir_t dir)
{
    if(!g_lcd_init)
        return;
    //dir |= 0x08;  //excahnge RGB
    dir = ((lcd_ctl.dir & DIR_RGB2BRG) == DIR_RGB2BRG) ? (dir | DIR_RGB2BRG) : dir;

#if defined(CONFIG_BOARD_TWATCH)
    lcd_ctl.width = g_lcd_w - 1;
    lcd_ctl.height = g_lcd_h - 1;
    switch (dir)
    {
    case DIR_XY_RLUD:
        lcd_ctl.start_offset_w = 0;
        lcd_ctl.start_offset_h = 0;
        break;
    case DIR_YX_LRUD:
        lcd_ctl.start_offset_w = 0;
        lcd_ctl.start_offset_h = 0;
        break;
    case DIR_YX_RLDU:  
        lcd_ctl.start_offset_w = 80;
        lcd_ctl.start_offset_h = 0;
        break;
    case DIR_XY_LRDU:   
        lcd_ctl.start_offset_w = 0;
        lcd_ctl.start_offset_h = 80;
        break;  
    default:
        break;
    }
#else
    if (dir & DIR_XY_MASK)
    {
        lcd_ctl.width = g_lcd_w - 1;
        lcd_ctl.height = g_lcd_h - 1;
        lcd_ctl.start_offset_w = lcd_ctl.start_offset_w1;
        lcd_ctl.start_offset_h = lcd_ctl.start_offset_h1;
    }
    else
    {
        lcd_ctl.width = g_lcd_h - 1;
        lcd_ctl.height = g_lcd_w - 1;
        lcd_ctl.start_offset_w = lcd_ctl.start_offset_w0;
        lcd_ctl.start_offset_h = lcd_ctl.start_offset_h0;
    }
#endif
    tft_write_command(MEMORY_ACCESS_CTL);
    tft_write_byte((uint8_t *)&dir, 1);
}

void lcd_set_offset(uint16_t offset_w, uint16_t offset_h)
{
    lcd_ctl.start_offset_w = offset_w;
    lcd_ctl.start_offset_h = offset_h;
}

static uint32_t lcd_freq = CONFIG_LCD_DEFAULT_FREQ;
void lcd_set_freq(uint32_t freq)
{
    tft_set_clk_freq(freq);
    lcd_freq = freq;
}

uint32_t lcd_get_freq()
{
    return lcd_freq;
}

void lcd_set_area(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    uint8_t data[4] = {0};

    x1 += lcd_ctl.start_offset_w;
    x2 += lcd_ctl.start_offset_w;
    y1 += lcd_ctl.start_offset_h;
    y2 += lcd_ctl.start_offset_h;

    data[0] = (uint8_t)(x1 >> 8);
    data[1] = (uint8_t)(x1);
    data[2] = (uint8_t)(x2 >> 8);
    data[3] = (uint8_t)(x2);
    tft_write_command(HORIZONTAL_ADDRESS_SET);
    tft_write_byte(data, 4);

    data[0] = (uint8_t)(y1 >> 8);
    data[1] = (uint8_t)(y1);
    data[2] = (uint8_t)(y2 >> 8);
    data[3] = (uint8_t)(y2);
    tft_write_command(VERTICAL_ADDRESS_SET);
    tft_write_byte(data, 4);

    tft_write_command(MEMORY_WRITE);
}

void lcd_draw_point(uint16_t x, uint16_t y, uint16_t color)
{
    lcd_set_area(x, y, x, y);
    tft_write_half(&color, 1);
}

void lcd_draw_char(uint16_t x, uint16_t y, char c, uint16_t color)
{
    uint8_t i = 0;
    uint8_t j = 0;
    uint8_t data = 0;

    for (i = 0; i < 16; i++)
    {
        data = ascii0816[c * 16 + i];
        for (j = 0; j < 8; j++)
        {
            if (data & 0x80)
                lcd_draw_point(x + j, y, color);
            data <<= 1;
        }
        y++;
    }
}

void lcd_draw_string(uint16_t x, uint16_t y, char *str, uint16_t color)
{
    while (*str)
    {
        lcd_draw_char(x, y, *str, color);
        str++;
        x += 8;
    }
}

void lcd_clear(uint16_t color)
{
    uint32_t data = ((uint32_t)color << 16) | (uint32_t)color;
    lcd_set_area(0, 0, lcd_ctl.width, lcd_ctl.height);
    tft_fill_data(&data, g_lcd_h * g_lcd_w / 2);
}

void lcd_fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
	if((x1 == x2) || (y1 == y2))
        return;
 	uint32_t data = ((uint32_t)color << 16) | (uint32_t)color;
    lcd_set_area(x1, y1, x2-1, y2-1);
    tft_fill_data(&data, (x2 - x1) * (y2 - y1) / 2);
}

void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t width, uint16_t color)
{
    uint32_t data_buf[640] = {0};
    uint32_t *p = data_buf;
    uint32_t data;
    uint32_t index = 0;

    data = ((uint32_t)color << 16) | color;
    for (index = 0; index < 160 * width; index++)
        *p++ = data;

    lcd_set_area(x1, y1, x2, y1 + width - 1);
    tft_write_byte((uint8_t*)data_buf, ((x2 - x1 + 1) * width + 1)*2 );
    lcd_set_area(x1, y2 - width + 1, x2, y2);
    tft_write_byte((uint8_t*)data_buf, ((x2 - x1 + 1) * width + 1)*2 );
    lcd_set_area(x1, y1, x1 + width - 1, y2);
    tft_write_byte((uint8_t*)data_buf, ((y2 - y1 + 1) * width + 1)*2 );
    lcd_set_area(x2 - width + 1, y1, x2, y2);
    tft_write_byte((uint8_t*)data_buf, ((y2 - y1 + 1) * width + 1)*2 );
}

void lcd_draw_buf(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint32_t *ptr)
{
    int i,len;
    lcd_set_area(x, y, x + width - 1, y + height - 1);
    len = width * height;
    if(len > (g_lcd_w * g_lcd_h))
        len = g_lcd_w * g_lcd_h;
    for(i=0;i<len/2;i++)
    {
        g_lcd_display_buff[i*2] = ((uint16_t *)ptr)[i*2+1];
        g_lcd_display_buff[i*2+1] = ((uint16_t *)ptr)[i*2];
    }
    tft_write_word((uint32_t*)g_lcd_display_buff, len / 2);
}
