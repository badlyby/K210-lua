#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <fpioa.h>
#include <sysctl.h>
#include <unistd.h>
#include "lstate.h"
#include "lcd.h"

static int lua_lcd_init (lua_State *L) {
    int width = 320, height = 240, dir = DIR_YX_LRUD, freq = 15000000, offset_w = 0, offset_h = 0, offset_w1 = 0, offset_h1 = 0;
    bool oct = true;
    bool inv_color = false;
    if(lua_gettop(L) >= 1) width = luaL_checkinteger(L, 1);
    if(lua_gettop(L) >= 2) height = luaL_checkinteger(L, 2);
    if(lua_gettop(L) >= 3) dir = luaL_checkinteger(L, 3);
    if(lua_gettop(L) >= 4) inv_color = lua_toboolean(L, 4);
    if(lua_gettop(L) >= 5) freq = luaL_checkinteger(L, 5);
    if(lua_gettop(L) >= 6) oct = lua_toboolean(L, 6);
    if(lua_gettop(L) >= 7) offset_w = luaL_checkinteger(L, 7);
    if(lua_gettop(L) >= 8) offset_h = luaL_checkinteger(L, 8);
    if(lua_gettop(L) >= 9) offset_w1 = luaL_checkinteger(L, 9);
    if(lua_gettop(L) >= 10) offset_h1 = luaL_checkinteger(L, 10);
    lua_lock();
    sysctl_set_spi0_dvp_data(1);
    fpioa_set_function(39, FUNC_SPI0_SCLK);
    lcd_init(freq, oct, offset_w, offset_h, offset_w1, offset_h1, inv_color, dir, width, height);
    lcd_set_direction(dir);
    lcd_set_area(0, 0, width - 1, height - 1);
    lua_unlock();
    return 0;
}

static int lua_lcd_clear (lua_State *L) {
    uint16_t color = 0;
    if(lua_gettop(L) >= 1) color = luaL_checkinteger(L, 1);
    lua_lock();
    sysctl_set_spi0_dvp_data(1);
    fpioa_set_function(39, FUNC_SPI0_SCLK);
    lcd_clear(color);
    lua_unlock();
    return 0;
}

static int lua_lcd_destory (lua_State *L) {
    lcd_destroy();
    return 0;
}

static int lua_lcd_set_freq (lua_State *L) {
    int freq = 0;
    if(lua_gettop(L) == 1)
    {
        freq = luaL_checkinteger(L, 1);
        lua_lock();
        sysctl_set_spi0_dvp_data(1);
        fpioa_set_function(39, FUNC_SPI0_SCLK);
        lcd_set_freq(freq);
        lua_unlock();
    }
    return 0;
}

static int lua_lcd_get_freq (lua_State *L) {
    lua_pushinteger(L, lcd_get_freq());
    return 1;
}

static int lua_lcd_set_dir (lua_State *L) {
    int dir = 0;
    if(lua_gettop(L) == 1)
    {
        dir = luaL_checkinteger(L, 1);
        lua_lock();
        sysctl_set_spi0_dvp_data(1);
        fpioa_set_function(39, FUNC_SPI0_SCLK);
        lcd_set_direction(dir);
        lua_unlock();
    }
    return 0;
}

static int lua_lcd_set_area (lua_State *L) {
    int x1,y1,x2,y2;
    if(lua_gettop(L) == 4)
    {
        x1 = luaL_checkinteger(L, 1);
        y1 = luaL_checkinteger(L, 2);
        x2 = luaL_checkinteger(L, 3);
        y2 = luaL_checkinteger(L, 4);
        lua_lock();
        sysctl_set_spi0_dvp_data(1);
        fpioa_set_function(39, FUNC_SPI0_SCLK);
        lcd_set_area(x1, y1, x2, y2);
        lua_unlock();
    }
    return 0;
}

static int lua_lcd_draw_point (lua_State *L) {
    int x, y;
    uint16_t color;
    if(lua_gettop(L) == 3)
    {
        x = luaL_checkinteger(L, 1);
        y = luaL_checkinteger(L, 2);
        color = luaL_checkinteger(L, 3);
        lua_lock();
        sysctl_set_spi0_dvp_data(1);
        fpioa_set_function(39, FUNC_SPI0_SCLK);
        lcd_draw_point(x, y, color);
        lua_unlock();
    }
    return 0;
}

static int lua_lcd_draw_string (lua_State *L) {
    int x, y;
    uint16_t color;
    const char *str;
    if(lua_gettop(L) == 4)
    {
        x = luaL_checkinteger(L, 1);
        y = luaL_checkinteger(L, 2);
        str = luaL_checkstring(L, 3);
        color = luaL_checkinteger(L, 4);
        lua_lock();
        sysctl_set_spi0_dvp_data(1);
        fpioa_set_function(39, FUNC_SPI0_SCLK);
        lcd_draw_string(x, y, (char *)str, color);
        lua_unlock();
    }
    return 0;
}

static int lua_lcd_fill_rectangle (lua_State *L) {
    int x1, y1, x2, y2;
    uint16_t color;
    if(lua_gettop(L) == 5)
    {
        x1 = luaL_checkinteger(L, 1);
        y1 = luaL_checkinteger(L, 2);
        x2 = luaL_checkinteger(L, 3);
        y2 = luaL_checkinteger(L, 4);
        color = luaL_checkinteger(L, 5);
        lua_lock();
        sysctl_set_spi0_dvp_data(1);
        fpioa_set_function(39, FUNC_SPI0_SCLK);
        lcd_fill_rectangle(x1, y1, x2, y2, color);
        lua_unlock();
    }
    return 0;
}

static int lua_lcd_draw_rectangle (lua_State *L) {
    int x1, y1, x2, y2, width;
    uint16_t color;
    if(lua_gettop(L) == 6)
    {
        x1 = luaL_checkinteger(L, 1);
        y1 = luaL_checkinteger(L, 2);
        x2 = luaL_checkinteger(L, 3);
        y2 = luaL_checkinteger(L, 4);
        width = luaL_checkinteger(L, 5);
        color = luaL_checkinteger(L, 6);
        lua_lock();
        sysctl_set_spi0_dvp_data(1);
        fpioa_set_function(39, FUNC_SPI0_SCLK);
        lcd_draw_rectangle(x1, y1, x2, y2, width, color);
        lua_unlock();
    }
    return 0;
}

static int lua_lcd_set_offset (lua_State *L) {
    int w,h;
    if(lua_gettop(L) == 2)
    {
        w = luaL_checkinteger(L, 1);
        h = luaL_checkinteger(L, 2);
        lua_lock();
        sysctl_set_spi0_dvp_data(1);
        fpioa_set_function(39, FUNC_SPI0_SCLK);
        lcd_set_offset(w, h);
        lua_unlock();
    }
    return 0;
}

static int lua_lcd_get_width (lua_State *L) {
    lua_pushinteger(L, lcd_get_width());
    return 1;
}

static int lua_lcd_get_height (lua_State *L) {
    lua_pushinteger(L, lcd_get_height());
    return 1;
}

static int lua_lcd_rgb565 (lua_State *L) {
    int r,g,b,color;
    if(lua_gettop(L) == 1)
    {
        color = luaL_checkinteger(L, 1);
        r = (color >> 16) & 0xFF;
        g = (color >> 8) & 0xFF;
        b = color & 0xFF;
        r <<= 8;
        r &= 0xF800;
        g <<= 3;
        g &= 0x7E0;
        b >>= 3;
        b &= 0x1F;
        color = r | g | b;
        lua_pushinteger(L, color);
        return 1;
    }
    if(lua_gettop(L) == 3)
    {
        r = luaL_checkinteger(L, 1);
        g = luaL_checkinteger(L, 2);
        b = luaL_checkinteger(L, 3);
        r <<= 8;
        r &= 0xF800;
        g <<= 3;
        g &= 0x7E0;
        b >>= 3;
        b &= 0x1F;
        color = r | g | b;
        lua_pushinteger(L, color);
        return 1;
    }
    return 0;
}

static int lua_lcd_draw_buf (lua_State *L) {
    int x, y, width, height;
    const uint32_t *ptr = NULL;
    if(lua_gettop(L) == 5)
    {
        x = luaL_checkinteger(L, 1);
        y = luaL_checkinteger(L, 2);
        width = luaL_checkinteger(L, 3);
        height = luaL_checkinteger(L, 4);
        ptr = lua_topointer(L, 5);
        if(ptr != NULL)
        {
            lua_lock();
            sysctl_set_spi0_dvp_data(1);
            fpioa_set_function(39, FUNC_SPI0_SCLK);
            lcd_draw_buf(x, y, width, height, ptr);
            lua_unlock();
        }
    }
    return 0;
}

static const luaL_Reg lcdlib[] = {
    {"init", lua_lcd_init},
    {"clear", lua_lcd_clear},
    {"destory", lua_lcd_destory},
    {"set_freq", lua_lcd_set_freq},
    {"get_freq", lua_lcd_get_freq},
    {"set_dir", lua_lcd_set_dir},
    {"set_area", lua_lcd_set_area},
    {"draw_point", lua_lcd_draw_point},
    {"draw_string", lua_lcd_draw_string},
    {"fill_rectangle", lua_lcd_fill_rectangle},
    {"draw_rectangle", lua_lcd_draw_rectangle},
    {"set_offset", lua_lcd_set_offset},
    {"get_width", lua_lcd_get_width},
    {"get_height", lua_lcd_get_height},
    {"draw_buf", lua_lcd_draw_buf},
    {"rgb565", lua_lcd_rgb565},
    {NULL, NULL}
};

LUAMOD_API int luaopen_lcd (lua_State *L) {
    luaL_newlib(L, lcdlib);  /* new module */
    return 1;
}
