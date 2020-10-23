#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <fpioa.h>
#include <uart.h>

#define LUA_UARTHANDLE	"UARTHANDLE"

typedef struct {
    int num;
} struct_uart;

#define uart_num(L)	((struct_uart *)luaL_checkudata(L, 1, LUA_UARTHANDLE))

static struct_uart *newport (lua_State *L)
{
    struct_uart* p = (struct_uart *)lua_newuserdatauv(L, sizeof(struct_uart), 0);
    luaL_setmetatable(L, LUA_UARTHANDLE);
    return p;
}

static int lua_uart_init (lua_State *L) {
    int num = luaL_checkinteger(L, 1);
    if(((num-1) >= UART_DEVICE_1) && ((num-1) < UART_DEVICE_MAX))
    {
        struct_uart* pu = newport(L);
        pu->num = num-1;
    return 1;
    }
    return 0;
}

static int lua_uart_configure(lua_State *L) {
    struct_uart *uart = uart_num(L);
    int baud_rate, data_width, stopbit, parity;
    baud_rate = luaL_checkinteger(L, 2);
    data_width = luaL_checkinteger(L, 3);
    stopbit = luaL_checkinteger(L, 4);
    parity = luaL_checkinteger(L, 5);
    uart_configure(uart->num, baud_rate, data_width, stopbit, parity);
    return 0;
}

static int lua_uart_send_fifo(lua_State *L) {
    struct_uart *uart = uart_num(L);
    int lv;
    lv = luaL_checkinteger(L, 2);
    uart_set_send_trigger(uart->num, lv);
    return 0;
}

static int lua_uart_recv_fifo(lua_State *L) {
    struct_uart *uart = uart_num(L);
    int lv;
    lv = luaL_checkinteger(L, 2);
    uart_set_receive_trigger(uart->num, lv);
    return 0;
}

static lua_State *Lcb = NULL;
int uart_callback(void *ctx)
{
    if(Lcb != NULL)
    {
        lua_settop(Lcb,0);
        lua_pushfunction(Lcb, (lua_Integer)ctx);
        if(lua_pcall(Lcb, 0, 0, 0)) lua_error(Lcb);
    }
    return 0;
}

static int lua_uart_irq_register(lua_State *L) {
    struct_uart *uart = uart_num(L);
    int mode, ctx, priority;
    mode = luaL_checkinteger(L, 2);
    ctx = lua_tofunction(L, 3);
    priority = luaL_checkinteger(L, 4);
    Lcb = L;
    uart_irq_register(uart->num, mode, uart_callback, (void *)ctx, priority);
    return 0;
}

static int lua_uart_irq_unregister(lua_State *L) {
    struct_uart *uart = uart_num(L);
    int mode;
    mode = luaL_checkinteger(L, 2);
    uart_irq_unregister(uart->num, mode);
    return 0;
}

static int lua_uart_send_data(lua_State *L) {
    struct_uart *uart = uart_num(L);
    size_t l;
    const char *s = luaL_checklstring(L, 2, &l);
    uart_send_data(uart->num, s, l);
    return 0;
}

static int lua_uart_receive_data(lua_State *L) {
    struct_uart *uart = uart_num(L);
    int nr;
    luaL_Buffer b;
    luaL_buffinit(L, &b);
    do {  /* read file in chunks of LUAL_BUFFERSIZE bytes */
        char *p = luaL_prepbuffer(&b);
        nr = uart_receive_data(uart->num, p, LUAL_BUFFERSIZE);
        luaL_addsize(&b, nr);
    } while (nr == LUAL_BUFFERSIZE);
    luaL_pushresult(&b);  /* close buffer */
    return 1;
}

static int lua_uart_set_work_mode(lua_State *L) {
    struct_uart *uart = uart_num(L);
    int work_mode;
    work_mode = luaL_checkinteger(L, 2);
    uart_set_work_mode(uart->num, work_mode);
    return 0;
}

static int lua_uart_set_rede_polarity(lua_State *L) {
    struct_uart *uart = uart_num(L);
    int rede, polarity;
    rede = luaL_checkinteger(L, 2);
    polarity = luaL_checkinteger(L, 3);
    uart_set_rede_polarity(uart->num, rede, polarity);
    return 0;
}

static int lua_uart_set_rede_enable(lua_State *L) {
    struct_uart *uart = uart_num(L);
    int rede;
    bool enable;
    rede = luaL_checkinteger(L, 2);
    enable = lua_toboolean(L, 3);
    uart_set_rede_enable(uart->num, rede, enable);
    return 0;
}

static int lua_uart_set_tat(lua_State *L) {
    struct_uart *uart = uart_num(L);
    int mode;
    lua_Integer time;
    mode = luaL_checkinteger(L, 2);
    time = luaL_checkinteger(L, 3);
    uart_set_tat(uart->num, mode, time);
    return 0;
}

static int lua_uart_set_det(lua_State *L) {
    struct_uart *uart = uart_num(L);
    int mode;
    lua_Integer time;
    mode = luaL_checkinteger(L, 2);
    time = luaL_checkinteger(L, 3);
    uart_set_det(uart->num, mode, time);
    return 0;
}

static int uart_gc (lua_State *L) {
    lua_uart_irq_unregister(L);
    return 0;
}

static int uart_tostring (lua_State *L) {
    struct_uart *uart = uart_num(L);
    lua_pushfstring(L, "UART%d",uart->num);
    lua_pushfstring(L, "NONE");
    return 1;
}

static const luaL_Reg uartlib[] = {
    {"init", lua_uart_init},
    {"UART_STOP_1", NULL},
    {"UART_STOP_1_5", NULL},
    {"UART_STOP_2", NULL},
    {"UART_PARITY_NONE", NULL},
    {"UART_PARITY_ODD", NULL},
    {"UART_PARITY_EVEN", NULL},
    {"UART_SEND", NULL},
    {"UART_RECEIVE", NULL},
    {"UART_SEND_FIFO_0", NULL},
    {"UART_SEND_FIFO_2", NULL},
    {"UART_SEND_FIFO_4", NULL},
    {"UART_SEND_FIFO_8", NULL},
    {"UART_RECEIVE_FIFO_1", NULL},
    {"UART_RECEIVE_FIFO_4", NULL},
    {"UART_RECEIVE_FIFO_8", NULL},
    {"UART_RECEIVE_FIFO_14", NULL},
    {"UART_NORMAL", NULL},
    {"UART_IRDA", NULL},
    {"UART_RS485_FULL_DUPLEX", NULL},
    {"UART_RS485_HALF_DUPLEX", NULL},
    {"UART_LOW", NULL},
    {"UART_HIGH", NULL},
    {"UART_DE_ASSERTION", NULL},
    {"UART_DE_DE_ASSERTION", NULL},
    {"UART_DE_ALL", NULL},
    {"UART_DE_TO_RE", NULL},
    {"UART_RE_TO_DE", NULL},
    {"UART_TAT_ALL", NULL},
    {NULL, NULL}
};


static const luaL_Reg meth[] = {
    {"configure", lua_uart_configure},
    {"send_fifo", lua_uart_send_fifo},
    {"recv_fifo", lua_uart_recv_fifo},
    {"irq_register", lua_uart_irq_register},
    {"irq_unregister", lua_uart_irq_unregister},
    {"send_data", lua_uart_send_data},
    {"receive_data", lua_uart_receive_data},
    {"set_work_mode", lua_uart_set_work_mode},
    {"set_rede_polarity", lua_uart_set_rede_polarity},
    {"set_rede_enable", lua_uart_set_rede_enable},
    {"set_tat", lua_uart_set_tat},
    {"set_det", lua_uart_set_det},
    {NULL, NULL}
};

static const luaL_Reg metameth[] = {
  {"__index", NULL},  /* place holder */
  {"__gc", uart_gc},
  {"__close", uart_gc},
  {"__tostring", uart_tostring},
  {NULL, NULL}
};

static void createmeta (lua_State *L) {
    luaL_newmetatable(L, LUA_UARTHANDLE);  /* create metatable for file handles */
    luaL_setfuncs(L, metameth, 0);  /* add metamethods to new metatable */
    luaL_newlibtable(L, meth);  /* create method table */
    luaL_setfuncs(L, meth, 0);  /* add file methods to method table */
    lua_setfield(L, -2, "__index");  /* metatable.__index = method table */
    lua_pop(L, 1);  /* pop metatable */
}

LUAMOD_API int luaopen_uart (lua_State *L) {
    luaL_newlib(L, uartlib);  /* new module */
    lua_pushinteger(L, UART_STOP_1);
    lua_setfield(L, -2, "UART_STOP_1");
    lua_pushinteger(L, UART_STOP_1_5);
    lua_setfield(L, -2, "UART_STOP_1_5");
    lua_pushinteger(L, UART_STOP_2);
    lua_setfield(L, -2, "UART_STOP_2");
    lua_pushinteger(L, UART_PARITY_NONE);
    lua_setfield(L, -2, "UART_PARITY_NONE");
    lua_pushinteger(L, UART_PARITY_ODD);
    lua_setfield(L, -2, "UART_PARITY_ODD");
    lua_pushinteger(L, UART_PARITY_EVEN);
    lua_setfield(L, -2, "UART_PARITY_EVEN");
    lua_pushinteger(L, UART_SEND);
    lua_setfield(L, -2, "UART_SEND");
    lua_pushinteger(L, UART_RECEIVE);
    lua_setfield(L, -2, "UART_RECEIVE");
    lua_pushinteger(L, UART_SEND_FIFO_0);
    lua_setfield(L, -2, "UART_SEND_FIFO_0");
    lua_pushinteger(L, UART_SEND_FIFO_2);
    lua_setfield(L, -2, "UART_SEND_FIFO_2");
    lua_pushinteger(L, UART_SEND_FIFO_4);
    lua_setfield(L, -2, "UART_SEND_FIFO_4");
    lua_pushinteger(L, UART_SEND_FIFO_8);
    lua_setfield(L, -2, "UART_SEND_FIFO_8");
    lua_pushinteger(L, UART_RECEIVE_FIFO_1);
    lua_setfield(L, -2, "UART_RECEIVE_FIFO_1");
    lua_pushinteger(L, UART_RECEIVE_FIFO_4);
    lua_setfield(L, -2, "UART_RECEIVE_FIFO_4");
    lua_pushinteger(L, UART_RECEIVE_FIFO_8);
    lua_setfield(L, -2, "UART_RECEIVE_FIFO_8");
    lua_pushinteger(L, UART_RECEIVE_FIFO_14);
    lua_setfield(L, -2, "UART_RECEIVE_FIFO_14");
    lua_pushinteger(L, UART_NORMAL);
    lua_setfield(L, -2, "UART_NORMAL");
    lua_pushinteger(L, UART_IRDA);
    lua_setfield(L, -2, "UART_IRDA");
    lua_pushinteger(L, UART_RS485_FULL_DUPLEX);
    lua_setfield(L, -2, "UART_RS485_FULL_DUPLEX");
    lua_pushinteger(L, UART_RS485_HALF_DUPLEX);
    lua_setfield(L, -2, "UART_RS485_HALF_DUPLEX");
    lua_pushinteger(L, UART_LOW);
    lua_setfield(L, -2, "UART_LOW");
    lua_pushinteger(L, UART_HIGH);
    lua_setfield(L, -2, "UART_HIGH");
    lua_pushinteger(L, UART_DE_ASSERTION);
    lua_setfield(L, -2, "UART_DE_ASSERTION");
    lua_pushinteger(L, UART_DE_DE_ASSERTION);
    lua_setfield(L, -2, "UART_DE_DE_ASSERTION");
    lua_pushinteger(L, UART_DE_ALL);
    lua_setfield(L, -2, "UART_DE_ALL");
    lua_pushinteger(L, UART_DE_TO_RE);
    lua_setfield(L, -2, "UART_DE_TO_RE");
    lua_pushinteger(L, UART_RE_TO_DE);
    lua_setfield(L, -2, "UART_RE_TO_DE");
    lua_pushinteger(L, UART_TAT_ALL);
    lua_setfield(L, -2, "UART_TAT_ALL");
    createmeta(L);
    return 1;
}
