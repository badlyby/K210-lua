#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <fpioa.h>
#include <i2c.h>
#include "lstate.h"

#define LUA_I2CHANDLE	"I2CHANDLE"
typedef struct {
    lua_State *L;
    lua_Integer on_receive;
    lua_Integer on_transmit;
    lua_Integer on_event;
    i2c_slave_handler_t handler;
} LUA_I2C_DEV;

static LUA_I2C_DEV lua_i2c_dev[3];

static void on_receive0(uint32_t data)
{
    if(lua_i2c_dev[0].L != NULL)
    {
        lua_settop(lua_i2c_dev[0].L, 0);
        lua_pushfunction(lua_i2c_dev[0].L, lua_i2c_dev[0].on_receive);
        if(lua_pcall(lua_i2c_dev[0].L, 0, 0, 0)) lua_error(lua_i2c_dev[0].L);
    }
}

static uint32_t on_transmit0()
{
    uint32_t ret = 0;
    if(lua_i2c_dev[0].L != NULL)
    {
        lua_settop(lua_i2c_dev[0].L, 0);
        lua_pushfunction(lua_i2c_dev[0].L, lua_i2c_dev[0].on_transmit);
        if(lua_pcall(lua_i2c_dev[0].L, 0, 0, 0)) lua_error(lua_i2c_dev[0].L);
    }
    return ret;
}

static void on_event0(i2c_event_t event)
{
    if(lua_i2c_dev[0].L != NULL)
    {
        lua_settop(lua_i2c_dev[0].L, 0);
        lua_pushfunction(lua_i2c_dev[0].L, lua_i2c_dev[0].on_event);
        if(lua_pcall(lua_i2c_dev[0].L, 0, 0, 0)) lua_error(lua_i2c_dev[0].L);
    }
}

static void on_receive1(uint32_t data)
{
    if(lua_i2c_dev[1].L != NULL)
    {
        lua_settop(lua_i2c_dev[1].L, 0);
        lua_pushfunction(lua_i2c_dev[1].L, lua_i2c_dev[1].on_receive);
        if(lua_pcall(lua_i2c_dev[1].L, 0, 0, 0)) lua_error(lua_i2c_dev[1].L);
    }
}

static uint32_t on_transmit1()
{
    uint32_t ret = 0;
    if(lua_i2c_dev[1].L != NULL)
    {
        lua_settop(lua_i2c_dev[1].L, 0);
        lua_pushfunction(lua_i2c_dev[1].L, lua_i2c_dev[1].on_transmit);
        if(lua_pcall(lua_i2c_dev[1].L, 0, 0, 0)) lua_error(lua_i2c_dev[1].L);
    }
    return ret;
}

static void on_event1(i2c_event_t event)
{
    if(lua_i2c_dev[1].L != NULL)
    {
        lua_settop(lua_i2c_dev[1].L, 0);
        lua_pushfunction(lua_i2c_dev[1].L, lua_i2c_dev[1].on_event);
        if(lua_pcall(lua_i2c_dev[1].L, 0, 0, 0)) lua_error(lua_i2c_dev[1].L);
    }
}

static void on_receive2(uint32_t data)
{
    if(lua_i2c_dev[2].L != NULL)
    {
        lua_settop(lua_i2c_dev[2].L, 0);
        lua_pushfunction(lua_i2c_dev[2].L, lua_i2c_dev[2].on_receive);
        if(lua_pcall(lua_i2c_dev[2].L, 0, 0, 0)) lua_error(lua_i2c_dev[2].L);
    }
}

static uint32_t on_transmit2()
{
    uint32_t ret = 0;
    if(lua_i2c_dev[2].L != NULL)
    {
        lua_settop(lua_i2c_dev[2].L, 0);
        lua_pushfunction(lua_i2c_dev[2].L, lua_i2c_dev[2].on_transmit);
        if(lua_pcall(lua_i2c_dev[2].L, 0, 0, 0)) lua_error(lua_i2c_dev[2].L);
    }
    return ret;
}

static void on_event2(i2c_event_t event)
{
    if(lua_i2c_dev[2].L != NULL)
    {
        lua_settop(lua_i2c_dev[2].L, 0);
        lua_pushfunction(lua_i2c_dev[2].L, lua_i2c_dev[2].on_event);
        if(lua_pcall(lua_i2c_dev[2].L, 0, 0, 0)) lua_error(lua_i2c_dev[2].L);
    }
}

typedef struct {
    int num;
    int addr;
    int addr_wid;
    int clk;
} struct_i2c;

#define i2c_dev(L)	((struct_i2c *)luaL_checkudata(L, 1, LUA_I2CHANDLE))

static struct_i2c *newport (lua_State *L)
{
    struct_i2c* p = (struct_i2c *)lua_newuserdatauv(L, sizeof(struct_i2c), 0);
    luaL_setmetatable(L, LUA_I2CHANDLE);
    return p;
}

static int lua_i2c_init (lua_State *L) {
    if(lua_gettop(L) >= 1) {
        struct_i2c *i2c = newport(L);
        i2c->num = luaL_checkinteger(L, 1);
        if(lua_gettop(L) >= 2) i2c->clk = luaL_checkinteger(L, 2);
        else i2c->clk = 100000;
        if(lua_gettop(L) >= 3) i2c->addr_wid = luaL_checkinteger(L, 3);
        else i2c->addr_wid = 7;
        if(lua_gettop(L) >= 4) i2c->addr = luaL_checkinteger(L, 4);
        else i2c->addr = 0;
        i2c_init(i2c->num, i2c->addr, i2c->addr_wid, i2c->clk);
        return 1;
    }
    return 0;
}

static int lua_i2c_init_slave (lua_State *L) {
    if(lua_gettop(L) == 6) {
        struct_i2c *i2c = newport(L);
        i2c->num = luaL_checkinteger(L, 1);
        i2c->addr = luaL_checkinteger(L, 2);
        i2c->addr_wid = luaL_checkinteger(L, 3);
        lua_i2c_dev[i2c->num].on_receive = lua_tofunction(L, 4);
        lua_i2c_dev[i2c->num].on_transmit = lua_tofunction(L, 5);
        lua_i2c_dev[i2c->num].on_event = lua_tofunction(L, 6);
        if(lua_i2c_dev[i2c->num].L == NULL)
            lua_i2c_dev[i2c->num].L = lua_newthread(L);
        i2c_init_as_slave(i2c->num, i2c->addr, i2c->addr_wid, &(lua_i2c_dev[i2c->num].handler));
        return 1;
    }
    return 0;
}

static int lua_i2c_recv (lua_State *L) {
    struct_i2c *i2c = i2c_dev(L);
    size_t cmd_len = 0, rx_len = 0;
    const char *cmd_buff = NULL;
    uint8_t *rx_buff = NULL;
    luaL_Buffer b;
    if(lua_gettop(L) >= 2)
    {
        rx_len = luaL_checkinteger(L, 2);
        if(lua_gettop(L) == 3)
        {
            cmd_buff = luaL_tolstring(L, 3, &cmd_len);
        }
        luaL_buffinit(L, &b);
        rx_buff = (uint8_t *)luaL_prepbuffsize(&b, rx_len);  /* prepare buffer to read whole block */
        i2c_recv_data(i2c->num, (const uint8_t *)cmd_buff, cmd_len, rx_buff, rx_len);
        luaL_addsize(&b, rx_len);
        luaL_pushresult(&b);  /* close buffer */
        return (rx_len > 0);
    }
    return 0;
}

static int lua_i2c_set_addr(lua_State *L) {
    struct_i2c *i2c = i2c_dev(L);
    if(lua_gettop(L) >= 2)
    {
        i2c->addr = luaL_checkinteger(L, 2);
        if(lua_gettop(L) == 3)
        {
            i2c->addr_wid = luaL_checkinteger(L, 3);
        }
        if(lua_i2c_dev[i2c->num].L != NULL)
        {
            i2c_init_as_slave(i2c->num, i2c->addr, i2c->addr_wid, &(lua_i2c_dev[i2c->num].handler));
        }
        else
        {
            i2c_init(i2c->num, i2c->addr, i2c->addr_wid, i2c->clk);
        }
    }
    return 0;
}

static int lua_i2c_send(lua_State *L) {
    struct_i2c *i2c = i2c_dev(L);
    size_t tx_len = 0;
    const char *tx_buff = NULL;
    if(lua_gettop(L) == 2)
    {
        tx_buff = luaL_tolstring(L, 2, &tx_len);
        i2c_send_data(i2c->num, (const uint8_t *)tx_buff, tx_len);
    }
    return 0;
}

static int i2c_gc (lua_State *L) {
    struct_i2c *i2c = i2c_dev(L);
    if(lua_i2c_dev[i2c->num].L != NULL)
    {
        plic_irq_disable(IRQN_I2C0_INTERRUPT + i2c->num);
        luaE_freethread(L, lua_i2c_dev[i2c->num].L);
    }
    return 0;
}

static int i2c_tostring (lua_State *L) {
    struct_i2c *i2c = i2c_dev(L);
    if(lua_i2c_dev[i2c->num].L != NULL)
        lua_pushfstring(L, "i2c%d, mode = slave, addr=%d, address_width=%d", i2c->num, i2c->addr, i2c->addr_wid);
    else
        lua_pushfstring(L, "i2c%d, mode = master, addr=%d, address_width=%d, clk=%d", i2c->num, i2c->addr, i2c->addr_wid, i2c->clk);
    return 1;
}

static const luaL_Reg i2clib[] = {
    {"init", lua_i2c_init},
    {"init_slave", lua_i2c_init_slave},
    {NULL, NULL}
};


static const luaL_Reg meth[] = {
    {"set_addr", lua_i2c_set_addr},
    {"send", lua_i2c_send},
    {"recv", lua_i2c_recv},
    {NULL, NULL}
};

static const luaL_Reg metameth[] = {
  {"__index", NULL},  /* place holder */
  {"__gc", i2c_gc},
  {"__close", i2c_gc},
  {"__tostring", i2c_tostring},
  {NULL, NULL}
};

static void createmeta (lua_State *L) {
    luaL_newmetatable(L, LUA_I2CHANDLE);  /* create metatable for file handles */
    luaL_setfuncs(L, metameth, 0);  /* add metamethods to new metatable */
    luaL_newlibtable(L, meth);  /* create method table */
    luaL_setfuncs(L, meth, 0);  /* add file methods to method table */
    lua_setfield(L, -2, "__index");  /* metatable.__index = method table */
    lua_pop(L, 1);  /* pop metatable */
}

LUAMOD_API int luaopen_i2c (lua_State *L) {
    lua_i2c_dev[0].L = NULL;
    lua_i2c_dev[0].on_receive = 0;
    lua_i2c_dev[0].on_transmit = 0;
    lua_i2c_dev[0].on_event = 0;
    lua_i2c_dev[0].handler.on_receive = on_receive0;
    lua_i2c_dev[0].handler.on_transmit = on_transmit0;
    lua_i2c_dev[0].handler.on_event = on_event0;
    lua_i2c_dev[1].L = NULL;
    lua_i2c_dev[1].on_receive = 0;
    lua_i2c_dev[1].on_transmit = 0;
    lua_i2c_dev[1].on_event = 0;
    lua_i2c_dev[1].handler.on_receive = on_receive1;
    lua_i2c_dev[1].handler.on_transmit = on_transmit1;
    lua_i2c_dev[1].handler.on_event = on_event1;
    lua_i2c_dev[2].L = NULL;
    lua_i2c_dev[2].on_receive = 0;
    lua_i2c_dev[2].on_transmit = 0;
    lua_i2c_dev[2].on_event = 0;
    lua_i2c_dev[2].handler.on_receive = on_receive2;
    lua_i2c_dev[2].handler.on_transmit = on_transmit2;
    lua_i2c_dev[2].handler.on_event = on_event2;
    luaL_newlib(L, i2clib);  /* new module */
    createmeta(L);
    return 1;
}
