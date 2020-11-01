#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <fpioa.h>
#include <spi.h>
#include "lstate.h"

#define LUA_SPIHANDLE	"SPIHANDLE"

typedef struct {
    int num;
    int cs;
    int mode;
    int frame;
    int dlen;
    int rate;
} struct_spi;

#define spi_dev(L)	((struct_spi *)luaL_checkudata(L, 1, LUA_SPIHANDLE))

static struct_spi *newport (lua_State *L)
{
    struct_spi* p = (struct_spi *)lua_newuserdatauv(L, sizeof(struct_spi), 0);
    luaL_setmetatable(L, LUA_SPIHANDLE);
    return p;
}

static int lua_spi_init (lua_State *L) {
    if(lua_gettop(L) >= 1) {
        struct_spi *spi = newport(L);
        spi->num = luaL_checkinteger(L, 1);
        if(lua_gettop(L) >= 2) spi->cs = luaL_checkinteger(L, 2);
        else spi->cs = 0;
        if(lua_gettop(L) >= 3) spi->mode = luaL_checkinteger(L, 3);
        else spi->mode = SPI_WORK_MODE_0;
        if(lua_gettop(L) >= 4) spi->frame = luaL_checkinteger(L, 4);
        else spi->frame = SPI_FF_STANDARD;
        if(lua_gettop(L) >= 5) spi->dlen = luaL_checkinteger(L, 5);
        else spi->dlen = 8;
        if(lua_gettop(L) >= 6) spi->rate = luaL_checkinteger(L, 6);
        else spi->rate = 6000000;
        spi_init(spi->num, spi->mode, spi->frame, spi->dlen, 0);
        spi_set_clk_rate(spi->num, spi->rate);
        return 1;
    }
    return 0;
}

static int lua_spi_read (lua_State *L) {
    struct_spi *spi = spi_dev(L);
    size_t cmd_len = 0, rx_len = 0;
    const char *cmd_buff = NULL, *rx_buff = NULL;
    luaL_Buffer b;
    if(lua_gettop(L) >= 2)
    {
        rx_len = luaL_checkinteger(L, 2);
        if(lua_gettop(L) == 3)
        {
            cmd_buff = luaL_tolstring(L, 3, &cmd_len);
        }
        spi_init(spi->num, spi->mode, spi->frame, spi->dlen, 0);
        luaL_buffinit(L, &b);
        rx_buff = luaL_prepbuffsize(&b, rx_len);  /* prepare buffer to read whole block */
        spi_receive_data_standard(spi->num, spi->cs, (uint8_t *)cmd_buff, cmd_len, (uint8_t *)rx_buff, rx_len);
        luaL_addsize(&b, rx_len);
        luaL_pushresult(&b);  /* close buffer */
        return (rx_len > 0);
    }
    return 0;
}

static int lua_spi_write(lua_State *L) {
    struct_spi *spi = spi_dev(L);
    size_t cmd_len = 0, tx_len = 0;
    const char *cmd_buff = NULL, *tx_buff = NULL;
    if(lua_gettop(L) >= 2)
    {
        tx_buff = luaL_tolstring(L, 2, &tx_len);
        if(lua_gettop(L) == 3)
        {
            cmd_buff = luaL_tolstring(L, 3, &cmd_len);
        }
        spi_init(spi->num, spi->mode, spi->frame, spi->dlen, 0);
        spi_send_data_standard(spi->num, spi->cs, (uint8_t *)cmd_buff, cmd_len, (uint8_t *)tx_buff, tx_len);
    }
    return 0;
}

static int spi_gc (lua_State *L) {
    return 0;
}

static int spi_tostring (lua_State *L) {
    struct_spi *spi = spi_dev(L);
    lua_pushfstring(L, "SPI%d, chip_select=%d, mode=%d, frame_format=%d, data_length=%d, clk_rate=%d", spi->num, spi->cs, spi->mode, spi->frame, spi->dlen, spi->rate);
    return 1;
}

static const luaL_Reg spilib[] = {
    {"init", lua_spi_init},
    {"SPI_FF_STANDARD", NULL},
    {"SPI_FF_DUAL", NULL},
    {"SPI_FF_QUAD", NULL},
    {"SPI_FF_OCTAL", NULL},
    {NULL, NULL}
};


static const luaL_Reg meth[] = {
    {"read", lua_spi_read},
    {"write", lua_spi_write},
    {NULL, NULL}
};

static const luaL_Reg metameth[] = {
  {"__index", NULL},  /* place holder */
  {"__gc", spi_gc},
  {"__close", spi_gc},
  {"__tostring", spi_tostring},
  {NULL, NULL}
};

static void createmeta (lua_State *L) {
    luaL_newmetatable(L, LUA_SPIHANDLE);  /* create metatable for file handles */
    luaL_setfuncs(L, metameth, 0);  /* add metamethods to new metatable */
    luaL_newlibtable(L, meth);  /* create method table */
    luaL_setfuncs(L, meth, 0);  /* add file methods to method table */
    lua_setfield(L, -2, "__index");  /* metatable.__index = method table */
    lua_pop(L, 1);  /* pop metatable */
}

LUAMOD_API int luaopen_spi (lua_State *L) {
    luaL_newlib(L, spilib);  /* new module */
    lua_pushinteger(L, SPI_FF_STANDARD);
    lua_setfield(L, -2, "SPI_FF_STANDARD");
    lua_pushinteger(L, SPI_FF_DUAL);
    lua_setfield(L, -2, "SPI_FF_DUAL");
    lua_pushinteger(L, SPI_FF_QUAD);
    lua_setfield(L, -2, "SPI_FF_QUAD");
    lua_pushinteger(L, SPI_FF_OCTAL);
    lua_setfield(L, -2, "SPI_FF_OCTAL");
    createmeta(L);
    return 1;
}
