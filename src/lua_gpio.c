#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <fpioa.h>
#include <gpio.h>
#include <gpiohs.h>

#define LUA_GPIOHANDLE	"GPIOHANDLE"

typedef struct {
    int func;
} struct_gpio;

#define gpio_pin(L)	((struct_gpio *)luaL_checkudata(L, 1, LUA_GPIOHANDLE))

static struct_gpio *newpin (lua_State *L)
{
    struct_gpio* p = (struct_gpio *)lua_newuserdatauv(L, sizeof(struct_gpio), 0);
    luaL_setmetatable(L, LUA_GPIOHANDLE);
    return p;
}

static int lua_gpio_pin (lua_State *L) {
    int func = luaL_checkinteger(L, 1);
    struct_gpio* p = newpin(L);
    p->func = func;
    return 1;
}

static int lua_gpio_init (lua_State *L) {
    lua_pushinteger(L, gpio_init());
    return 1;
}


static int lua_gpio_set_drive_mode(lua_State *L) {
    struct_gpio *gpio = gpio_pin(L);
    int mode;
    mode = luaL_checkinteger(L, 2);
    if((gpio->func >= FUNC_GPIO0) && (gpio->func <= FUNC_GPIO7))
    {
        gpio_set_drive_mode(gpio->func - FUNC_GPIO0, mode);
        return 0;
    }
    else if((gpio->func >= FUNC_GPIOHS0) && (gpio->func <= FUNC_GPIOHS31))
    {
        gpiohs_set_drive_mode(gpio->func - FUNC_GPIOHS0, mode);
        return 0;
    }
    else
    {
        return 0;
    }
}

static int lua_gpio_set_pin(lua_State *L) {
    struct_gpio *gpio = gpio_pin(L);
    int value;
    value = luaL_checkinteger(L, 2);
    if((gpio->func >= FUNC_GPIO0) && (gpio->func <= FUNC_GPIO7))
    {
        gpio_set_pin(gpio->func - FUNC_GPIO0, value);
        return 0;
    }
    else if((gpio->func >= FUNC_GPIOHS0) && (gpio->func <= FUNC_GPIOHS31))
    {
        gpiohs_set_pin(gpio->func - FUNC_GPIOHS0, value);
        return 0;
    }
    else
    {
        return 0;
    }
}
/*get_pin
gpiohs_get_pin
lua_set_pin
gpiohs_set_pin


gpiohs_set_pin_edge

gpiohs_set_irq

gpiohs_irq_register


gpiohs_irq_unregister
*/

static int gpio_gc (lua_State *L) {
    return 0;
}

static int gpio_tostring (lua_State *L) {
    struct_gpio *gpio = gpio_pin(L);
    if((gpio->func >= FUNC_GPIO0) && (gpio->func <= FUNC_GPIO7))
    {
        lua_pushfstring(L, "FUNC_GPIO%d",gpio->func - FUNC_GPIO0);
        return 1;
    }
    else if((gpio->func >= FUNC_GPIOHS0) && (gpio->func <= FUNC_GPIOHS31))
    {
        lua_pushfstring(L, "FUNC_GPIOHS%d",gpio->func - FUNC_GPIOHS0);
        return 1;
    }
    lua_pushfstring(L, "NONE");
    return 1;
}

static const luaL_Reg gpiolib[] = {
    {"pin", lua_gpio_pin},
    {"init", lua_gpio_init},
    {"GPIO_DM_INPUT", NULL},
    {"GPIO_DM_INPUT_PULL_DOWN", NULL},
    {"GPIO_DM_INPUT_PULL_UP", NULL},
    {"GPIO_DM_OUTPUT", NULL},
    {"GPIO_PE_NONE", NULL},
    {"GPIO_PE_FALLING", NULL},
    {"GPIO_PE_RISING", NULL},
    {"GPIO_PE_BOTH", NULL},
    {"GPIO_PE_LOW", NULL},
    {"GPIO_PE_HIGH", NULL},
    {"GPIO_PV_LOW", NULL},
    {"GPIO_PV_HIGH", NULL},
    {NULL, NULL}
};


static const luaL_Reg meth[] = {
    {"set_drive_mode",lua_gpio_set_drive_mode},
    {"set_pin", lua_gpio_set_pin},
    {NULL, NULL}
};

static const luaL_Reg metameth[] = {
  {"__index", NULL},  /* place holder */
  {"__gc", gpio_gc},
  {"__close", gpio_gc},
  {"__tostring", gpio_tostring},
  {NULL, NULL}
};

static void createmeta (lua_State *L) {
    luaL_newmetatable(L, LUA_GPIOHANDLE);  /* create metatable for file handles */
    luaL_setfuncs(L, metameth, 0);  /* add metamethods to new metatable */
    luaL_newlibtable(L, meth);  /* create method table */
    luaL_setfuncs(L, meth, 0);  /* add file methods to method table */
    lua_setfield(L, -2, "__index");  /* metatable.__index = method table */
    lua_pop(L, 1);  /* pop metatable */
}

LUAMOD_API int luaopen_gpio (lua_State *L) {
    luaL_newlib(L, gpiolib);  /* new module */
    lua_pushinteger(L, GPIO_DM_INPUT);
    lua_setfield(L, -2, "GPIO_DM_INPUT");
    lua_pushinteger(L, GPIO_DM_INPUT_PULL_DOWN);
    lua_setfield(L, -2, "GPIO_DM_INPUT_PULL_DOWN");
    lua_pushinteger(L, GPIO_DM_INPUT_PULL_UP);
    lua_setfield(L, -2, "GPIO_DM_INPUT_PULL_UP");
    lua_pushinteger(L, GPIO_DM_OUTPUT);
    lua_setfield(L, -2, "GPIO_DM_OUTPUT");
    lua_pushinteger(L, GPIO_PE_NONE);
    lua_setfield(L, -2, "GPIO_PE_NONE");
    lua_pushinteger(L, GPIO_PE_FALLING);
    lua_setfield(L, -2, "GPIO_PE_FALLING");
    lua_pushinteger(L, GPIO_PE_RISING);
    lua_setfield(L, -2, "GPIO_PE_RISING");
    lua_pushinteger(L, GPIO_PE_BOTH);
    lua_setfield(L, -2, "GPIO_PE_BOTH");
    lua_pushinteger(L, GPIO_PE_LOW);
    lua_setfield(L, -2, "GPIO_PE_LOW");
    lua_pushinteger(L, GPIO_PE_HIGH);
    lua_setfield(L, -2, "GPIO_PE_HIGH");
    lua_pushinteger(L, GPIO_PV_LOW);
    lua_setfield(L, -2, "GPIO_PV_LOW");
    lua_pushinteger(L, GPIO_PV_HIGH);
    lua_setfield(L, -2, "GPIO_PV_HIGH");
    createmeta(L);
    return 1;
}
