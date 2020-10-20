#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
#include <fpioa.h>

//int fpioa_get_io(int number, fpioa_io_config_t *cfg);
//int fpioa_set_io(int number, fpioa_io_config_t *cfg);
static int lua_fpioa_init (lua_State *L) {
  lua_pushinteger(L, fpioa_init());
  return 1;
}
static int lua_fpioa_set_function_raw (lua_State *L) {
  int ret, number, function;
  number = luaL_checkinteger(L, 1);
  function = luaL_checkinteger(L, 2);
  ret = fpioa_set_function_raw(number, function);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_fpioa_set_function (lua_State *L) {
  int ret, number, function;
  number = luaL_checkinteger(L, 1);
  function = luaL_checkinteger(L, 2);
  ret = fpioa_set_function(number, function);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_fpioa_set_tie_enable (lua_State *L) {
  int ret, function, enable;
  function = luaL_checkinteger(L, 1);
  enable = luaL_checkinteger(L, 2);
  ret = fpioa_set_tie_enable(function, enable);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_fpioa_set_tie_value (lua_State *L) {
  int ret, function, value;
  function = luaL_checkinteger(L, 1);
  value = luaL_checkinteger(L, 2);
  ret = fpioa_set_tie_value(function, value);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_fpioa_set_io_pull (lua_State *L) {
  int ret, number, pull;
  number = luaL_checkinteger(L, 1);
  pull = luaL_checkinteger(L, 2);
  ret = fpioa_set_io_pull(number, pull);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_fpioa_get_io_pull (lua_State *L) {
  int ret, number;
  number = luaL_checkinteger(L, 1);
  ret = fpioa_get_io_pull(number);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_fpioa_set_io_driving (lua_State *L) {
  int ret, number, driving;
  number = luaL_checkinteger(L, 1);
  driving = luaL_checkinteger(L, 2);
  ret = fpioa_set_io_driving(number, driving);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_fpioa_get_io_driving (lua_State *L) {
  int ret, number;
  number = luaL_checkinteger(L, 1);
  ret = fpioa_get_io_driving(number);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_fpioa_get_io_by_function (lua_State *L) {
  int ret, function;
  function = luaL_checkinteger(L, 1);
  ret = fpioa_get_io_by_function(function);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_fpioa_set_sl (lua_State *L) {
  int ret, number, sl_enable;
  number = luaL_checkinteger(L, 1);
  sl_enable = luaL_checkinteger(L, 2);
  ret = fpioa_set_sl(number, sl_enable);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_fpioa_set_st (lua_State *L) {
  int ret, number, st_enable;
  number = luaL_checkinteger(L, 1);
  st_enable = luaL_checkinteger(L, 2);
  ret = fpioa_set_st(number, st_enable);
  lua_pushinteger(L, ret);
  return 1;
}

static int lua_fpioa_set_oe_inv (lua_State *L) {
  int ret, number, inv_enable;
  number = luaL_checkinteger(L, 1);
  inv_enable = luaL_checkinteger(L, 2);
  ret = fpioa_set_oe_inv(number, inv_enable);
  lua_pushinteger(L, ret);
  return 1;
}

static const luaL_Reg fpioa_lib[] = {
  {"init", lua_fpioa_init},
  {"set_function_raw", lua_fpioa_set_function_raw},
  {"set_function", lua_fpioa_set_function},
  {"set_tie_enable", lua_fpioa_set_tie_enable},
  {"set_tie_value", lua_fpioa_set_tie_value},
  {"set_io_pull", lua_fpioa_set_io_pull},
  {"get_io_pull", lua_fpioa_get_io_pull},
  {"set_io_driving", lua_fpioa_set_io_driving},
  {"get_io_driving", lua_fpioa_get_io_driving},
  {"get_io_by_function", lua_fpioa_get_io_by_function},
  {"set_sl", lua_fpioa_set_sl},
  {"set_st", lua_fpioa_set_st},
  {"set_oe_inv", lua_fpioa_set_oe_inv},
  {"FUNC_JTAG_TCLK", NULL},
  {"FUNC_JTAG_TDI", NULL},
  {"FUNC_JTAG_TMS", NULL},
  {"FUNC_JTAG_TDO", NULL},
  {"FUNC_SPI0_D0", NULL},
  {"FUNC_SPI0_D1", NULL},
  {"FUNC_SPI0_D2", NULL},
  {"FUNC_SPI0_D3", NULL},
  {"FUNC_SPI0_D4", NULL},
  {"FUNC_SPI0_D5", NULL},
  {"FUNC_SPI0_D6", NULL},
  {"FUNC_SPI0_D7", NULL},
  {"FUNC_SPI0_SS0", NULL},
  {"FUNC_SPI0_SS1", NULL},
  {"FUNC_SPI0_SS2", NULL},
  {"FUNC_SPI0_SS3", NULL},
  {"FUNC_SPI0_ARB", NULL},
  {"FUNC_SPI0_SCLK", NULL},
  {"FUNC_UARTHS_RX", NULL},
  {"FUNC_UARTHS_TX", NULL},
  {"FUNC_RESV6", NULL},
  {"FUNC_RESV7", NULL},
  {"FUNC_CLK_SPI1", NULL},
  {"FUNC_CLK_I2C1", NULL},
  {"FUNC_GPIOHS0", NULL},
  {"FUNC_GPIOHS1", NULL},
  {"FUNC_GPIOHS2", NULL},
  {"FUNC_GPIOHS3", NULL},
  {"FUNC_GPIOHS4", NULL},
  {"FUNC_GPIOHS5", NULL},
  {"FUNC_GPIOHS6", NULL},
  {"FUNC_GPIOHS7", NULL},
  {"FUNC_GPIOHS8", NULL},
  {"FUNC_GPIOHS9", NULL},
  {"FUNC_GPIOHS10", NULL},
  {"FUNC_GPIOHS11", NULL},
  {"FUNC_GPIOHS12", NULL},
  {"FUNC_GPIOHS13", NULL},
  {"FUNC_GPIOHS14", NULL},
  {"FUNC_GPIOHS15", NULL},
  {"FUNC_GPIOHS16", NULL},
  {"FUNC_GPIOHS17", NULL},
  {"FUNC_GPIOHS18", NULL},
  {"FUNC_GPIOHS19", NULL},
  {"FUNC_GPIOHS20", NULL},
  {"FUNC_GPIOHS21", NULL},
  {"FUNC_GPIOHS22", NULL},
  {"FUNC_GPIOHS23", NULL},
  {"FUNC_GPIOHS24", NULL},
  {"FUNC_GPIOHS25", NULL},
  {"FUNC_GPIOHS26", NULL},
  {"FUNC_GPIOHS27", NULL},
  {"FUNC_GPIOHS28", NULL},
  {"FUNC_GPIOHS29", NULL},
  {"FUNC_GPIOHS30", NULL},
  {"FUNC_GPIOHS31", NULL},
  {"FUNC_GPIO0", NULL},
  {"FUNC_GPIO1", NULL},
  {"FUNC_GPIO2", NULL},
  {"FUNC_GPIO3", NULL},
  {"FUNC_GPIO4", NULL},
  {"FUNC_GPIO5", NULL},
  {"FUNC_GPIO6", NULL},
  {"FUNC_GPIO7", NULL},
  {"FUNC_UART1_RX", NULL},
  {"FUNC_UART1_TX", NULL},
  {"FUNC_UART2_RX", NULL},
  {"FUNC_UART2_TX", NULL},
  {"FUNC_UART3_RX", NULL},
  {"FUNC_UART3_TX", NULL},
  {"FUNC_SPI1_D0", NULL},
  {"FUNC_SPI1_D1", NULL},
  {"FUNC_SPI1_D2", NULL},
  {"FUNC_SPI1_D3", NULL},
  {"FUNC_SPI1_D4", NULL},
  {"FUNC_SPI1_D5", NULL},
  {"FUNC_SPI1_D6", NULL},
  {"FUNC_SPI1_D7", NULL},
  {"FUNC_SPI1_SS0", NULL},
  {"FUNC_SPI1_SS1", NULL},
  {"FUNC_SPI1_SS2", NULL},
  {"FUNC_SPI1_SS3", NULL},
  {"FUNC_SPI1_ARB", NULL},
  {"FUNC_SPI1_SCLK", NULL},
  {"FUNC_SPI_SLAVE_D0", NULL},
  {"FUNC_SPI_SLAVE_SS", NULL},
  {"FUNC_SPI_SLAVE_SCLK", NULL},
  {"FUNC_I2S0_MCLK", NULL},
  {"FUNC_I2S0_SCLK", NULL},
  {"FUNC_I2S0_WS", NULL},
  {"FUNC_I2S0_IN_D0", NULL},
  {"FUNC_I2S0_IN_D1", NULL},
  {"FUNC_I2S0_IN_D2", NULL},
  {"FUNC_I2S0_IN_D3", NULL},
  {"FUNC_I2S0_OUT_D0", NULL},
  {"FUNC_I2S0_OUT_D1", NULL},
  {"FUNC_I2S0_OUT_D2", NULL},
  {"FUNC_I2S0_OUT_D3", NULL},
  {"FUNC_I2S1_MCLK", NULL},
  {"FUNC_I2S1_SCLK", NULL},
  {"FUNC_I2S1_WS", NULL},
  {"FUNC_I2S1_IN_D0", NULL},
  {"FUNC_I2S1_IN_D1", NULL},
  {"FUNC_I2S1_IN_D2", NULL},
  {"FUNC_I2S1_IN_D3", NULL},
  {"FUNC_I2S1_OUT_D0", NULL},
  {"FUNC_I2S1_OUT_D1", NULL},
  {"FUNC_I2S1_OUT_D2", NULL},
  {"FUNC_I2S1_OUT_D3", NULL},
  {"FUNC_I2S2_MCLK", NULL},
  {"FUNC_I2S2_SCLK", NULL},
  {"FUNC_I2S2_WS", NULL},
  {"FUNC_I2S2_IN_D0", NULL},
  {"FUNC_I2S2_IN_D1", NULL},
  {"FUNC_I2S2_IN_D2", NULL},
  {"FUNC_I2S2_IN_D3", NULL},
  {"FUNC_I2S2_OUT_D0", NULL},
  {"FUNC_I2S2_OUT_D1", NULL},
  {"FUNC_I2S2_OUT_D2", NULL},
  {"FUNC_I2S2_OUT_D3", NULL},
  {"FUNC_RESV0", NULL},
  {"FUNC_RESV1", NULL},
  {"FUNC_RESV2", NULL},
  {"FUNC_RESV3", NULL},
  {"FUNC_RESV4", NULL},
  {"FUNC_RESV5", NULL},
  {"FUNC_I2C0_SCLK", NULL},
  {"FUNC_I2C0_SDA", NULL},
  {"FUNC_I2C1_SCLK", NULL},
  {"FUNC_I2C1_SDA", NULL},
  {"FUNC_I2C2_SCLK", NULL},
  {"FUNC_I2C2_SDA", NULL},
  {"FUNC_CMOS_XCLK", NULL},
  {"FUNC_CMOS_RST", NULL},
  {"FUNC_CMOS_PWDN", NULL},
  {"FUNC_CMOS_VSYNC", NULL},
  {"FUNC_CMOS_HREF", NULL},
  {"FUNC_CMOS_PCLK", NULL},
  {"FUNC_CMOS_D0", NULL},
  {"FUNC_CMOS_D1", NULL},
  {"FUNC_CMOS_D2", NULL},
  {"FUNC_CMOS_D3", NULL},
  {"FUNC_CMOS_D4", NULL},
  {"FUNC_CMOS_D5", NULL},
  {"FUNC_CMOS_D6", NULL},
  {"FUNC_CMOS_D7", NULL},
  {"FUNC_SCCB_SCLK", NULL},
  {"FUNC_SCCB_SDA", NULL},
  {"FUNC_UART1_CTS", NULL},
  {"FUNC_UART1_DSR", NULL},
  {"FUNC_UART1_DCD", NULL},
  {"FUNC_UART1_RI", NULL},
  {"FUNC_UART1_SIR_IN", NULL},
  {"FUNC_UART1_DTR", NULL},
  {"FUNC_UART1_RTS", NULL},
  {"FUNC_UART1_OUT2", NULL},
  {"FUNC_UART1_OUT1", NULL},
  {"FUNC_UART1_SIR_OUT", NULL},
  {"FUNC_UART1_BAUD", NULL},
  {"FUNC_UART1_RE", NULL},
  {"FUNC_UART1_DE", NULL},
  {"FUNC_UART1_RS485_EN", NULL},
  {"FUNC_UART2_CTS", NULL},
  {"FUNC_UART2_DSR", NULL},
  {"FUNC_UART2_DCD", NULL},
  {"FUNC_UART2_RI", NULL},
  {"FUNC_UART2_SIR_IN", NULL},
  {"FUNC_UART2_DTR", NULL},
  {"FUNC_UART2_RTS", NULL},
  {"FUNC_UART2_OUT2", NULL},
  {"FUNC_UART2_OUT1", NULL},
  {"FUNC_UART2_SIR_OUT", NULL},
  {"FUNC_UART2_BAUD", NULL},
  {"FUNC_UART2_RE", NULL},
  {"FUNC_UART2_DE", NULL},
  {"FUNC_UART2_RS485_EN", NULL},
  {"FUNC_UART3_CTS", NULL},
  {"FUNC_UART3_DSR", NULL},
  {"FUNC_UART3_DCD", NULL},
  {"FUNC_UART3_RI", NULL},
  {"FUNC_UART3_SIR_IN", NULL},
  {"FUNC_UART3_DTR", NULL},
  {"FUNC_UART3_RTS", NULL},
  {"FUNC_UART3_OUT2", NULL},
  {"FUNC_UART3_OUT1", NULL},
  {"FUNC_UART3_SIR_OUT", NULL},
  {"FUNC_UART3_BAUD", NULL},
  {"FUNC_UART3_RE", NULL},
  {"FUNC_UART3_DE", NULL},
  {"FUNC_UART3_RS485_EN", NULL},
  {"FUNC_TIMER0_TOGGLE1", NULL},
  {"FUNC_TIMER0_TOGGLE2", NULL},
  {"FUNC_TIMER0_TOGGLE3", NULL},
  {"FUNC_TIMER0_TOGGLE4", NULL},
  {"FUNC_TIMER1_TOGGLE1", NULL},
  {"FUNC_TIMER1_TOGGLE2", NULL},
  {"FUNC_TIMER1_TOGGLE3", NULL},
  {"FUNC_TIMER1_TOGGLE4", NULL},
  {"FUNC_TIMER2_TOGGLE1", NULL},
  {"FUNC_TIMER2_TOGGLE2", NULL},
  {"FUNC_TIMER2_TOGGLE3", NULL},
  {"FUNC_TIMER2_TOGGLE4", NULL},
  {"FUNC_CLK_SPI2", NULL},
  {"FUNC_CLK_I2C2", NULL},
  {"FUNC_INTERNAL0", NULL},
  {"FUNC_INTERNAL1", NULL},
  {"FUNC_INTERNAL2", NULL},
  {"FUNC_INTERNAL3", NULL},
  {"FUNC_INTERNAL4", NULL},
  {"FUNC_INTERNAL5", NULL},
  {"FUNC_INTERNAL6", NULL},
  {"FUNC_INTERNAL7", NULL},
  {"FUNC_INTERNAL8", NULL},
  {"FUNC_INTERNAL9", NULL},
  {"FUNC_INTERNAL10", NULL},
  {"FUNC_INTERNAL11", NULL},
  {"FUNC_INTERNAL12", NULL},
  {"FUNC_INTERNAL13", NULL},
  {"FUNC_INTERNAL14", NULL},
  {"FUNC_INTERNAL15", NULL},
  {"FUNC_INTERNAL16", NULL},
  {"FUNC_INTERNAL17", NULL},
  {"FUNC_CONSTANT", NULL},
  {"FUNC_INTERNAL18", NULL},
  {"FUNC_DEBUG0", NULL},
  {"FUNC_DEBUG1", NULL},
  {"FUNC_DEBUG2", NULL},
  {"FUNC_DEBUG3", NULL},
  {"FUNC_DEBUG4", NULL},
  {"FUNC_DEBUG5", NULL},
  {"FUNC_DEBUG6", NULL},
  {"FUNC_DEBUG7", NULL},
  {"FUNC_DEBUG8", NULL},
  {"FUNC_DEBUG9", NULL},
  {"FUNC_DEBUG10", NULL},
  {"FUNC_DEBUG11", NULL},
  {"FUNC_DEBUG12", NULL},
  {"FUNC_DEBUG13", NULL},
  {"FUNC_DEBUG14", NULL},
  {"FUNC_DEBUG15", NULL},
  {"FUNC_DEBUG16", NULL},
  {"FUNC_DEBUG17", NULL},
  {"FUNC_DEBUG18", NULL},
  {"FUNC_DEBUG19", NULL},
  {"FUNC_DEBUG20", NULL},
  {"FUNC_DEBUG21", NULL},
  {"FUNC_DEBUG22", NULL},
  {"FUNC_DEBUG23", NULL},
  {"FUNC_DEBUG24", NULL},
  {"FUNC_DEBUG25", NULL},
  {"FUNC_DEBUG26", NULL},
  {"FUNC_DEBUG27", NULL},
  {"FUNC_DEBUG28", NULL},
  {"FUNC_DEBUG29", NULL},
  {"FUNC_DEBUG30", NULL},
  {"FUNC_DEBUG31", NULL},
  {"FUNC_MAX", NULL},
  {"FPIOA_PULL_NONE", NULL},
  {"FPIOA_PULL_DOWN", NULL},
  {"FPIOA_PULL_UP", NULL},
  {"FPIOA_PULL_MAX", NULL},
  {"FPIOA_DRIVING_0", NULL},
  {"FPIOA_DRIVING_1", NULL},
  {"FPIOA_DRIVING_2", NULL},
  {"FPIOA_DRIVING_3", NULL},
  {"FPIOA_DRIVING_4", NULL},
  {"FPIOA_DRIVING_5", NULL},
  {"FPIOA_DRIVING_6", NULL},
  {"FPIOA_DRIVING_7", NULL},
  {"FPIOA_DRIVING_8", NULL},
  {"FPIOA_DRIVING_9", NULL},
  {"FPIOA_DRIVING_10", NULL},
  {"FPIOA_DRIVING_11", NULL},
  {"FPIOA_DRIVING_12", NULL},
  {"FPIOA_DRIVING_13", NULL},
  {"FPIOA_DRIVING_14", NULL},
  {"FPIOA_DRIVING_15", NULL},
  {"FPIOA_DRIVING_MA", NULL},
  {NULL, NULL}
};

LUAMOD_API int luaopen_fpioa (lua_State *L) {
  luaL_newlib(L, fpioa_lib);
  lua_pushinteger(L, FUNC_JTAG_TCLK);
  lua_setfield(L, -2, "FUNC_JTAG_TCLK");
  lua_pushinteger(L, FUNC_JTAG_TDI);
  lua_setfield(L, -2, "FUNC_JTAG_TDI");
  lua_pushinteger(L, FUNC_JTAG_TMS);
  lua_setfield(L, -2, "FUNC_JTAG_TMS");
  lua_pushinteger(L, FUNC_JTAG_TDO);
  lua_setfield(L, -2, "FUNC_JTAG_TDO");
  lua_pushinteger(L, FUNC_SPI0_D0);
  lua_setfield(L, -2, "FUNC_SPI0_D0");
  lua_pushinteger(L, FUNC_SPI0_D1);
  lua_setfield(L, -2, "FUNC_SPI0_D1");
  lua_pushinteger(L, FUNC_SPI0_D2);
  lua_setfield(L, -2, "FUNC_SPI0_D2");
  lua_pushinteger(L, FUNC_SPI0_D3);
  lua_setfield(L, -2, "FUNC_SPI0_D3");
  lua_pushinteger(L, FUNC_SPI0_D4);
  lua_setfield(L, -2, "FUNC_SPI0_D4");
  lua_pushinteger(L, FUNC_SPI0_D5);
  lua_setfield(L, -2, "FUNC_SPI0_D5");
  lua_pushinteger(L, FUNC_SPI0_D6);
  lua_setfield(L, -2, "FUNC_SPI0_D6");
  lua_pushinteger(L, FUNC_SPI0_D7);
  lua_setfield(L, -2, "FUNC_SPI0_D7");
  lua_pushinteger(L, FUNC_SPI0_SS0);
  lua_setfield(L, -2, "FUNC_SPI0_SS0");
  lua_pushinteger(L, FUNC_SPI0_SS1);
  lua_setfield(L, -2, "FUNC_SPI0_SS1");
  lua_pushinteger(L, FUNC_SPI0_SS2);
  lua_setfield(L, -2, "FUNC_SPI0_SS2");
  lua_pushinteger(L, FUNC_SPI0_SS3);
  lua_setfield(L, -2, "FUNC_SPI0_SS3");
  lua_pushinteger(L, FUNC_SPI0_ARB);
  lua_setfield(L, -2, "FUNC_SPI0_ARB");
  lua_pushinteger(L, FUNC_SPI0_SCLK);
  lua_setfield(L, -2, "FUNC_SPI0_SCLK");
  lua_pushinteger(L, FUNC_UARTHS_RX);
  lua_setfield(L, -2, "FUNC_UARTHS_RX");
  lua_pushinteger(L, FUNC_UARTHS_TX);
  lua_setfield(L, -2, "FUNC_UARTHS_TX");
  lua_pushinteger(L, FUNC_RESV6);
  lua_setfield(L, -2, "FUNC_RESV6");
  lua_pushinteger(L, FUNC_RESV7);
  lua_setfield(L, -2, "FUNC_RESV7");
  lua_pushinteger(L, FUNC_CLK_SPI1);
  lua_setfield(L, -2, "FUNC_CLK_SPI1");
  lua_pushinteger(L, FUNC_CLK_I2C1);
  lua_setfield(L, -2, "FUNC_CLK_I2C1");
  lua_pushinteger(L, FUNC_GPIOHS0);
  lua_setfield(L, -2, "FUNC_GPIOHS0");
  lua_pushinteger(L, FUNC_GPIOHS1);
  lua_setfield(L, -2, "FUNC_GPIOHS1");
  lua_pushinteger(L, FUNC_GPIOHS2);
  lua_setfield(L, -2, "FUNC_GPIOHS2");
  lua_pushinteger(L, FUNC_GPIOHS3);
  lua_setfield(L, -2, "FUNC_GPIOHS3");
  lua_pushinteger(L, FUNC_GPIOHS4);
  lua_setfield(L, -2, "FUNC_GPIOHS4");
  lua_pushinteger(L, FUNC_GPIOHS5);
  lua_setfield(L, -2, "FUNC_GPIOHS5");
  lua_pushinteger(L, FUNC_GPIOHS6);
  lua_setfield(L, -2, "FUNC_GPIOHS6");
  lua_pushinteger(L, FUNC_GPIOHS7);
  lua_setfield(L, -2, "FUNC_GPIOHS7");
  lua_pushinteger(L, FUNC_GPIOHS8);
  lua_setfield(L, -2, "FUNC_GPIOHS8");
  lua_pushinteger(L, FUNC_GPIOHS9);
  lua_setfield(L, -2, "FUNC_GPIOHS9");
  lua_pushinteger(L, FUNC_GPIOHS10);
  lua_setfield(L, -2, "FUNC_GPIOHS10");
  lua_pushinteger(L, FUNC_GPIOHS11);
  lua_setfield(L, -2, "FUNC_GPIOHS11");
  lua_pushinteger(L, FUNC_GPIOHS12);
  lua_setfield(L, -2, "FUNC_GPIOHS12");
  lua_pushinteger(L, FUNC_GPIOHS13);
  lua_setfield(L, -2, "FUNC_GPIOHS13");
  lua_pushinteger(L, FUNC_GPIOHS14);
  lua_setfield(L, -2, "FUNC_GPIOHS14");
  lua_pushinteger(L, FUNC_GPIOHS15);
  lua_setfield(L, -2, "FUNC_GPIOHS15");
  lua_pushinteger(L, FUNC_GPIOHS16);
  lua_setfield(L, -2, "FUNC_GPIOHS16");
  lua_pushinteger(L, FUNC_GPIOHS17);
  lua_setfield(L, -2, "FUNC_GPIOHS17");
  lua_pushinteger(L, FUNC_GPIOHS18);
  lua_setfield(L, -2, "FUNC_GPIOHS18");
  lua_pushinteger(L, FUNC_GPIOHS19);
  lua_setfield(L, -2, "FUNC_GPIOHS19");
  lua_pushinteger(L, FUNC_GPIOHS20);
  lua_setfield(L, -2, "FUNC_GPIOHS20");
  lua_pushinteger(L, FUNC_GPIOHS21);
  lua_setfield(L, -2, "FUNC_GPIOHS21");
  lua_pushinteger(L, FUNC_GPIOHS22);
  lua_setfield(L, -2, "FUNC_GPIOHS22");
  lua_pushinteger(L, FUNC_GPIOHS23);
  lua_setfield(L, -2, "FUNC_GPIOHS23");
  lua_pushinteger(L, FUNC_GPIOHS24);
  lua_setfield(L, -2, "FUNC_GPIOHS24");
  lua_pushinteger(L, FUNC_GPIOHS25);
  lua_setfield(L, -2, "FUNC_GPIOHS25");
  lua_pushinteger(L, FUNC_GPIOHS26);
  lua_setfield(L, -2, "FUNC_GPIOHS26");
  lua_pushinteger(L, FUNC_GPIOHS27);
  lua_setfield(L, -2, "FUNC_GPIOHS27");
  lua_pushinteger(L, FUNC_GPIOHS28);
  lua_setfield(L, -2, "FUNC_GPIOHS28");
  lua_pushinteger(L, FUNC_GPIOHS29);
  lua_setfield(L, -2, "FUNC_GPIOHS29");
  lua_pushinteger(L, FUNC_GPIOHS30);
  lua_setfield(L, -2, "FUNC_GPIOHS30");
  lua_pushinteger(L, FUNC_GPIOHS31);
  lua_setfield(L, -2, "FUNC_GPIOHS31");
  lua_pushinteger(L, FUNC_GPIO0);
  lua_setfield(L, -2, "FUNC_GPIO0");
  lua_pushinteger(L, FUNC_GPIO1);
  lua_setfield(L, -2, "FUNC_GPIO1");
  lua_pushinteger(L, FUNC_GPIO2);
  lua_setfield(L, -2, "FUNC_GPIO2");
  lua_pushinteger(L, FUNC_GPIO3);
  lua_setfield(L, -2, "FUNC_GPIO3");
  lua_pushinteger(L, FUNC_GPIO4);
  lua_setfield(L, -2, "FUNC_GPIO4");
  lua_pushinteger(L, FUNC_GPIO5);
  lua_setfield(L, -2, "FUNC_GPIO5");
  lua_pushinteger(L, FUNC_GPIO6);
  lua_setfield(L, -2, "FUNC_GPIO6");
  lua_pushinteger(L, FUNC_GPIO7);
  lua_setfield(L, -2, "FUNC_GPIO7");
  lua_pushinteger(L, FUNC_UART1_RX);
  lua_setfield(L, -2, "FUNC_UART1_RX");
  lua_pushinteger(L, FUNC_UART1_TX);
  lua_setfield(L, -2, "FUNC_UART1_TX");
  lua_pushinteger(L, FUNC_UART2_RX);
  lua_setfield(L, -2, "FUNC_UART2_RX");
  lua_pushinteger(L, FUNC_UART2_TX);
  lua_setfield(L, -2, "FUNC_UART2_TX");
  lua_pushinteger(L, FUNC_UART3_RX);
  lua_setfield(L, -2, "FUNC_UART3_RX");
  lua_pushinteger(L, FUNC_UART3_TX);
  lua_setfield(L, -2, "FUNC_UART3_TX");
  lua_pushinteger(L, FUNC_SPI1_D0);
  lua_setfield(L, -2, "FUNC_SPI1_D0");
  lua_pushinteger(L, FUNC_SPI1_D1);
  lua_setfield(L, -2, "FUNC_SPI1_D1");
  lua_pushinteger(L, FUNC_SPI1_D2);
  lua_setfield(L, -2, "FUNC_SPI1_D2");
  lua_pushinteger(L, FUNC_SPI1_D3);
  lua_setfield(L, -2, "FUNC_SPI1_D3");
  lua_pushinteger(L, FUNC_SPI1_D4);
  lua_setfield(L, -2, "FUNC_SPI1_D4");
  lua_pushinteger(L, FUNC_SPI1_D5);
  lua_setfield(L, -2, "FUNC_SPI1_D5");
  lua_pushinteger(L, FUNC_SPI1_D6);
  lua_setfield(L, -2, "FUNC_SPI1_D6");
  lua_pushinteger(L, FUNC_SPI1_D7);
  lua_setfield(L, -2, "FUNC_SPI1_D7");
  lua_pushinteger(L, FUNC_SPI1_SS0);
  lua_setfield(L, -2, "FUNC_SPI1_SS0");
  lua_pushinteger(L, FUNC_SPI1_SS1);
  lua_setfield(L, -2, "FUNC_SPI1_SS1");
  lua_pushinteger(L, FUNC_SPI1_SS2);
  lua_setfield(L, -2, "FUNC_SPI1_SS2");
  lua_pushinteger(L, FUNC_SPI1_SS3);
  lua_setfield(L, -2, "FUNC_SPI1_SS3");
  lua_pushinteger(L, FUNC_SPI1_ARB);
  lua_setfield(L, -2, "FUNC_SPI1_ARB");
  lua_pushinteger(L, FUNC_SPI1_SCLK);
  lua_setfield(L, -2, "FUNC_SPI1_SCLK");
  lua_pushinteger(L, FUNC_SPI_SLAVE_D0);
  lua_setfield(L, -2, "FUNC_SPI_SLAVE_D0");
  lua_pushinteger(L, FUNC_SPI_SLAVE_SS);
  lua_setfield(L, -2, "FUNC_SPI_SLAVE_SS");
  lua_pushinteger(L, FUNC_SPI_SLAVE_SCLK);
  lua_setfield(L, -2, "FUNC_SPI_SLAVE_SCLK");
  lua_pushinteger(L, FUNC_I2S0_MCLK);
  lua_setfield(L, -2, "FUNC_I2S0_MCLK");
  lua_pushinteger(L, FUNC_I2S0_SCLK);
  lua_setfield(L, -2, "FUNC_I2S0_SCLK");
  lua_pushinteger(L, FUNC_I2S0_WS);
  lua_setfield(L, -2, "FUNC_I2S0_WS");
  lua_pushinteger(L, FUNC_I2S0_IN_D0);
  lua_setfield(L, -2, "FUNC_I2S0_IN_D0");
  lua_pushinteger(L, FUNC_I2S0_IN_D1);
  lua_setfield(L, -2, "FUNC_I2S0_IN_D1");
  lua_pushinteger(L, FUNC_I2S0_IN_D2);
  lua_setfield(L, -2, "FUNC_I2S0_IN_D2");
  lua_pushinteger(L, FUNC_I2S0_IN_D3);
  lua_setfield(L, -2, "FUNC_I2S0_IN_D3");
  lua_pushinteger(L, FUNC_I2S0_OUT_D0);
  lua_setfield(L, -2, "FUNC_I2S0_OUT_D0");
  lua_pushinteger(L, FUNC_I2S0_OUT_D1);
  lua_setfield(L, -2, "FUNC_I2S0_OUT_D1");
  lua_pushinteger(L, FUNC_I2S0_OUT_D2);
  lua_setfield(L, -2, "FUNC_I2S0_OUT_D2");
  lua_pushinteger(L, FUNC_I2S0_OUT_D3);
  lua_setfield(L, -2, "FUNC_I2S0_OUT_D3");
  lua_pushinteger(L, FUNC_I2S1_MCLK);
  lua_setfield(L, -2, "FUNC_I2S1_MCLK");
  lua_pushinteger(L, FUNC_I2S1_SCLK);
  lua_setfield(L, -2, "FUNC_I2S1_SCLK");
  lua_pushinteger(L, FUNC_I2S1_WS);
  lua_setfield(L, -2, "FUNC_I2S1_WS");
  lua_pushinteger(L, FUNC_I2S1_IN_D0);
  lua_setfield(L, -2, "FUNC_I2S1_IN_D0");
  lua_pushinteger(L, FUNC_I2S1_IN_D1);
  lua_setfield(L, -2, "FUNC_I2S1_IN_D1");
  lua_pushinteger(L, FUNC_I2S1_IN_D2);
  lua_setfield(L, -2, "FUNC_I2S1_IN_D2");
  lua_pushinteger(L, FUNC_I2S1_IN_D3);
  lua_setfield(L, -2, "FUNC_I2S1_IN_D3");
  lua_pushinteger(L, FUNC_I2S1_OUT_D0);
  lua_setfield(L, -2, "FUNC_I2S1_OUT_D0");
  lua_pushinteger(L, FUNC_I2S1_OUT_D1);
  lua_setfield(L, -2, "FUNC_I2S1_OUT_D1");
  lua_pushinteger(L, FUNC_I2S1_OUT_D2);
  lua_setfield(L, -2, "FUNC_I2S1_OUT_D2");
  lua_pushinteger(L, FUNC_I2S1_OUT_D3);
  lua_setfield(L, -2, "FUNC_I2S1_OUT_D3");
  lua_pushinteger(L, FUNC_I2S2_MCLK);
  lua_setfield(L, -2, "FUNC_I2S2_MCLK");
  lua_pushinteger(L, FUNC_I2S2_SCLK);
  lua_setfield(L, -2, "FUNC_I2S2_SCLK");
  lua_pushinteger(L, FUNC_I2S2_WS);
  lua_setfield(L, -2, "FUNC_I2S2_WS");
  lua_pushinteger(L, FUNC_I2S2_IN_D0);
  lua_setfield(L, -2, "FUNC_I2S2_IN_D0");
  lua_pushinteger(L, FUNC_I2S2_IN_D1);
  lua_setfield(L, -2, "FUNC_I2S2_IN_D1");
  lua_pushinteger(L, FUNC_I2S2_IN_D2);
  lua_setfield(L, -2, "FUNC_I2S2_IN_D2");
  lua_pushinteger(L, FUNC_I2S2_IN_D3);
  lua_setfield(L, -2, "FUNC_I2S2_IN_D3");
  lua_pushinteger(L, FUNC_I2S2_OUT_D0);
  lua_setfield(L, -2, "FUNC_I2S2_OUT_D0");
  lua_pushinteger(L, FUNC_I2S2_OUT_D1);
  lua_setfield(L, -2, "FUNC_I2S2_OUT_D1");
  lua_pushinteger(L, FUNC_I2S2_OUT_D2);
  lua_setfield(L, -2, "FUNC_I2S2_OUT_D2");
  lua_pushinteger(L, FUNC_I2S2_OUT_D3);
  lua_setfield(L, -2, "FUNC_I2S2_OUT_D3");
  lua_pushinteger(L, FUNC_RESV0);
  lua_setfield(L, -2, "FUNC_RESV0");
  lua_pushinteger(L, FUNC_RESV1);
  lua_setfield(L, -2, "FUNC_RESV1");
  lua_pushinteger(L, FUNC_RESV2);
  lua_setfield(L, -2, "FUNC_RESV2");
  lua_pushinteger(L, FUNC_RESV3);
  lua_setfield(L, -2, "FUNC_RESV3");
  lua_pushinteger(L, FUNC_RESV4);
  lua_setfield(L, -2, "FUNC_RESV4");
  lua_pushinteger(L, FUNC_RESV5);
  lua_setfield(L, -2, "FUNC_RESV5");
  lua_pushinteger(L, FUNC_I2C0_SCLK);
  lua_setfield(L, -2, "FUNC_I2C0_SCLK");
  lua_pushinteger(L, FUNC_I2C0_SDA);
  lua_setfield(L, -2, "FUNC_I2C0_SDA");
  lua_pushinteger(L, FUNC_I2C1_SCLK);
  lua_setfield(L, -2, "FUNC_I2C1_SCLK");
  lua_pushinteger(L, FUNC_I2C1_SDA);
  lua_setfield(L, -2, "FUNC_I2C1_SDA");
  lua_pushinteger(L, FUNC_I2C2_SCLK);
  lua_setfield(L, -2, "FUNC_I2C2_SCLK");
  lua_pushinteger(L, FUNC_I2C2_SDA);
  lua_setfield(L, -2, "FUNC_I2C2_SDA");
  lua_pushinteger(L, FUNC_CMOS_XCLK);
  lua_setfield(L, -2, "FUNC_CMOS_XCLK");
  lua_pushinteger(L, FUNC_CMOS_RST);
  lua_setfield(L, -2, "FUNC_CMOS_RST");
  lua_pushinteger(L, FUNC_CMOS_PWDN);
  lua_setfield(L, -2, "FUNC_CMOS_PWDN");
  lua_pushinteger(L, FUNC_CMOS_VSYNC);
  lua_setfield(L, -2, "FUNC_CMOS_VSYNC");
  lua_pushinteger(L, FUNC_CMOS_HREF);
  lua_setfield(L, -2, "FUNC_CMOS_HREF");
  lua_pushinteger(L, FUNC_CMOS_PCLK);
  lua_setfield(L, -2, "FUNC_CMOS_PCLK");
  lua_pushinteger(L, FUNC_CMOS_D0);
  lua_setfield(L, -2, "FUNC_CMOS_D0");
  lua_pushinteger(L, FUNC_CMOS_D1);
  lua_setfield(L, -2, "FUNC_CMOS_D1");
  lua_pushinteger(L, FUNC_CMOS_D2);
  lua_setfield(L, -2, "FUNC_CMOS_D2");
  lua_pushinteger(L, FUNC_CMOS_D3);
  lua_setfield(L, -2, "FUNC_CMOS_D3");
  lua_pushinteger(L, FUNC_CMOS_D4);
  lua_setfield(L, -2, "FUNC_CMOS_D4");
  lua_pushinteger(L, FUNC_CMOS_D5);
  lua_setfield(L, -2, "FUNC_CMOS_D5");
  lua_pushinteger(L, FUNC_CMOS_D6);
  lua_setfield(L, -2, "FUNC_CMOS_D6");
  lua_pushinteger(L, FUNC_CMOS_D7);
  lua_setfield(L, -2, "FUNC_CMOS_D7");
  lua_pushinteger(L, FUNC_SCCB_SCLK);
  lua_setfield(L, -2, "FUNC_SCCB_SCLK");
  lua_pushinteger(L, FUNC_SCCB_SDA);
  lua_setfield(L, -2, "FUNC_SCCB_SDA");
  lua_pushinteger(L, FUNC_UART1_CTS);
  lua_setfield(L, -2, "FUNC_UART1_CTS");
  lua_pushinteger(L, FUNC_UART1_DSR);
  lua_setfield(L, -2, "FUNC_UART1_DSR");
  lua_pushinteger(L, FUNC_UART1_DCD);
  lua_setfield(L, -2, "FUNC_UART1_DCD");
  lua_pushinteger(L, FUNC_UART1_RI);
  lua_setfield(L, -2, "FUNC_UART1_RI");
  lua_pushinteger(L, FUNC_UART1_SIR_IN);
  lua_setfield(L, -2, "FUNC_UART1_SIR_IN");
  lua_pushinteger(L, FUNC_UART1_DTR);
  lua_setfield(L, -2, "FUNC_UART1_DTR");
  lua_pushinteger(L, FUNC_UART1_RTS);
  lua_setfield(L, -2, "FUNC_UART1_RTS");
  lua_pushinteger(L, FUNC_UART1_OUT2);
  lua_setfield(L, -2, "FUNC_UART1_OUT2");
  lua_pushinteger(L, FUNC_UART1_OUT1);
  lua_setfield(L, -2, "FUNC_UART1_OUT1");
  lua_pushinteger(L, FUNC_UART1_SIR_OUT);
  lua_setfield(L, -2, "FUNC_UART1_SIR_OUT");
  lua_pushinteger(L, FUNC_UART1_BAUD);
  lua_setfield(L, -2, "FUNC_UART1_BAUD");
  lua_pushinteger(L, FUNC_UART1_RE);
  lua_setfield(L, -2, "FUNC_UART1_RE");
  lua_pushinteger(L, FUNC_UART1_DE);
  lua_setfield(L, -2, "FUNC_UART1_DE");
  lua_pushinteger(L, FUNC_UART1_RS485_EN);
  lua_setfield(L, -2, "FUNC_UART1_RS485_EN");
  lua_pushinteger(L, FUNC_UART2_CTS);
  lua_setfield(L, -2, "FUNC_UART2_CTS");
  lua_pushinteger(L, FUNC_UART2_DSR);
  lua_setfield(L, -2, "FUNC_UART2_DSR");
  lua_pushinteger(L, FUNC_UART2_DCD);
  lua_setfield(L, -2, "FUNC_UART2_DCD");
  lua_pushinteger(L, FUNC_UART2_RI);
  lua_setfield(L, -2, "FUNC_UART2_RI");
  lua_pushinteger(L, FUNC_UART2_SIR_IN);
  lua_setfield(L, -2, "FUNC_UART2_SIR_IN");
  lua_pushinteger(L, FUNC_UART2_DTR);
  lua_setfield(L, -2, "FUNC_UART2_DTR");
  lua_pushinteger(L, FUNC_UART2_RTS);
  lua_setfield(L, -2, "FUNC_UART2_RTS");
  lua_pushinteger(L, FUNC_UART2_OUT2);
  lua_setfield(L, -2, "FUNC_UART2_OUT2");
  lua_pushinteger(L, FUNC_UART2_OUT1);
  lua_setfield(L, -2, "FUNC_UART2_OUT1");
  lua_pushinteger(L, FUNC_UART2_SIR_OUT);
  lua_setfield(L, -2, "FUNC_UART2_SIR_OUT");
  lua_pushinteger(L, FUNC_UART2_BAUD);
  lua_setfield(L, -2, "FUNC_UART2_BAUD");
  lua_pushinteger(L, FUNC_UART2_RE);
  lua_setfield(L, -2, "FUNC_UART2_RE");
  lua_pushinteger(L, FUNC_UART2_DE);
  lua_setfield(L, -2, "FUNC_UART2_DE");
  lua_pushinteger(L, FUNC_UART2_RS485_EN);
  lua_setfield(L, -2, "FUNC_UART2_RS485_EN");
  lua_pushinteger(L, FUNC_UART3_CTS);
  lua_setfield(L, -2, "FUNC_UART3_CTS");
  lua_pushinteger(L, FUNC_UART3_DSR);
  lua_setfield(L, -2, "FUNC_UART3_DSR");
  lua_pushinteger(L, FUNC_UART3_DCD);
  lua_setfield(L, -2, "FUNC_UART3_DCD");
  lua_pushinteger(L, FUNC_UART3_RI);
  lua_setfield(L, -2, "FUNC_UART3_RI");
  lua_pushinteger(L, FUNC_UART3_SIR_IN);
  lua_setfield(L, -2, "FUNC_UART3_SIR_IN");
  lua_pushinteger(L, FUNC_UART3_DTR);
  lua_setfield(L, -2, "FUNC_UART3_DTR");
  lua_pushinteger(L, FUNC_UART3_RTS);
  lua_setfield(L, -2, "FUNC_UART3_RTS");
  lua_pushinteger(L, FUNC_UART3_OUT2);
  lua_setfield(L, -2, "FUNC_UART3_OUT2");
  lua_pushinteger(L, FUNC_UART3_OUT1);
  lua_setfield(L, -2, "FUNC_UART3_OUT1");
  lua_pushinteger(L, FUNC_UART3_SIR_OUT);
  lua_setfield(L, -2, "FUNC_UART3_SIR_OUT");
  lua_pushinteger(L, FUNC_UART3_BAUD);
  lua_setfield(L, -2, "FUNC_UART3_BAUD");
  lua_pushinteger(L, FUNC_UART3_RE);
  lua_setfield(L, -2, "FUNC_UART3_RE");
  lua_pushinteger(L, FUNC_UART3_DE);
  lua_setfield(L, -2, "FUNC_UART3_DE");
  lua_pushinteger(L, FUNC_UART3_RS485_EN);
  lua_setfield(L, -2, "FUNC_UART3_RS485_EN");
  lua_pushinteger(L, FUNC_TIMER0_TOGGLE1);
  lua_setfield(L, -2, "FUNC_TIMER0_TOGGLE1");
  lua_pushinteger(L, FUNC_TIMER0_TOGGLE2);
  lua_setfield(L, -2, "FUNC_TIMER0_TOGGLE2");
  lua_pushinteger(L, FUNC_TIMER0_TOGGLE3);
  lua_setfield(L, -2, "FUNC_TIMER0_TOGGLE3");
  lua_pushinteger(L, FUNC_TIMER0_TOGGLE4);
  lua_setfield(L, -2, "FUNC_TIMER0_TOGGLE4");
  lua_pushinteger(L, FUNC_TIMER1_TOGGLE1);
  lua_setfield(L, -2, "FUNC_TIMER1_TOGGLE1");
  lua_pushinteger(L, FUNC_TIMER1_TOGGLE2);
  lua_setfield(L, -2, "FUNC_TIMER1_TOGGLE2");
  lua_pushinteger(L, FUNC_TIMER1_TOGGLE3);
  lua_setfield(L, -2, "FUNC_TIMER1_TOGGLE3");
  lua_pushinteger(L, FUNC_TIMER1_TOGGLE4);
  lua_setfield(L, -2, "FUNC_TIMER1_TOGGLE4");
  lua_pushinteger(L, FUNC_TIMER2_TOGGLE1);
  lua_setfield(L, -2, "FUNC_TIMER2_TOGGLE1");
  lua_pushinteger(L, FUNC_TIMER2_TOGGLE2);
  lua_setfield(L, -2, "FUNC_TIMER2_TOGGLE2");
  lua_pushinteger(L, FUNC_TIMER2_TOGGLE3);
  lua_setfield(L, -2, "FUNC_TIMER2_TOGGLE3");
  lua_pushinteger(L, FUNC_TIMER2_TOGGLE4);
  lua_setfield(L, -2, "FUNC_TIMER2_TOGGLE4");
  lua_pushinteger(L, FUNC_CLK_SPI2);
  lua_setfield(L, -2, "FUNC_CLK_SPI2");
  lua_pushinteger(L, FUNC_CLK_I2C2);
  lua_setfield(L, -2, "FUNC_CLK_I2C2");
  lua_pushinteger(L, FUNC_INTERNAL0);
  lua_setfield(L, -2, "FUNC_INTERNAL0");
  lua_pushinteger(L, FUNC_INTERNAL1);
  lua_setfield(L, -2, "FUNC_INTERNAL1");
  lua_pushinteger(L, FUNC_INTERNAL2);
  lua_setfield(L, -2, "FUNC_INTERNAL2");
  lua_pushinteger(L, FUNC_INTERNAL3);
  lua_setfield(L, -2, "FUNC_INTERNAL3");
  lua_pushinteger(L, FUNC_INTERNAL4);
  lua_setfield(L, -2, "FUNC_INTERNAL4");
  lua_pushinteger(L, FUNC_INTERNAL5);
  lua_setfield(L, -2, "FUNC_INTERNAL5");
  lua_pushinteger(L, FUNC_INTERNAL6);
  lua_setfield(L, -2, "FUNC_INTERNAL6");
  lua_pushinteger(L, FUNC_INTERNAL7);
  lua_setfield(L, -2, "FUNC_INTERNAL7");
  lua_pushinteger(L, FUNC_INTERNAL8);
  lua_setfield(L, -2, "FUNC_INTERNAL8");
  lua_pushinteger(L, FUNC_INTERNAL9);
  lua_setfield(L, -2, "FUNC_INTERNAL9");
  lua_pushinteger(L, FUNC_INTERNAL10);
  lua_setfield(L, -2, "FUNC_INTERNAL10");
  lua_pushinteger(L, FUNC_INTERNAL11);
  lua_setfield(L, -2, "FUNC_INTERNAL11");
  lua_pushinteger(L, FUNC_INTERNAL12);
  lua_setfield(L, -2, "FUNC_INTERNAL12");
  lua_pushinteger(L, FUNC_INTERNAL13);
  lua_setfield(L, -2, "FUNC_INTERNAL13");
  lua_pushinteger(L, FUNC_INTERNAL14);
  lua_setfield(L, -2, "FUNC_INTERNAL14");
  lua_pushinteger(L, FUNC_INTERNAL15);
  lua_setfield(L, -2, "FUNC_INTERNAL15");
  lua_pushinteger(L, FUNC_INTERNAL16);
  lua_setfield(L, -2, "FUNC_INTERNAL16");
  lua_pushinteger(L, FUNC_INTERNAL17);
  lua_setfield(L, -2, "FUNC_INTERNAL17");
  lua_pushinteger(L, FUNC_CONSTANT);
  lua_setfield(L, -2, "FUNC_CONSTANT");
  lua_pushinteger(L, FUNC_INTERNAL18);
  lua_setfield(L, -2, "FUNC_INTERNAL18");
  lua_pushinteger(L, FUNC_DEBUG0);
  lua_setfield(L, -2, "FUNC_DEBUG0");
  lua_pushinteger(L, FUNC_DEBUG1);
  lua_setfield(L, -2, "FUNC_DEBUG1");
  lua_pushinteger(L, FUNC_DEBUG2);
  lua_setfield(L, -2, "FUNC_DEBUG2");
  lua_pushinteger(L, FUNC_DEBUG3);
  lua_setfield(L, -2, "FUNC_DEBUG3");
  lua_pushinteger(L, FUNC_DEBUG4);
  lua_setfield(L, -2, "FUNC_DEBUG4");
  lua_pushinteger(L, FUNC_DEBUG5);
  lua_setfield(L, -2, "FUNC_DEBUG5");
  lua_pushinteger(L, FUNC_DEBUG6);
  lua_setfield(L, -2, "FUNC_DEBUG6");
  lua_pushinteger(L, FUNC_DEBUG7);
  lua_setfield(L, -2, "FUNC_DEBUG7");
  lua_pushinteger(L, FUNC_DEBUG8);
  lua_setfield(L, -2, "FUNC_DEBUG8");
  lua_pushinteger(L, FUNC_DEBUG9);
  lua_setfield(L, -2, "FUNC_DEBUG9");
  lua_pushinteger(L, FUNC_DEBUG10);
  lua_setfield(L, -2, "FUNC_DEBUG10");
  lua_pushinteger(L, FUNC_DEBUG11);
  lua_setfield(L, -2, "FUNC_DEBUG11");
  lua_pushinteger(L, FUNC_DEBUG12);
  lua_setfield(L, -2, "FUNC_DEBUG12");
  lua_pushinteger(L, FUNC_DEBUG13);
  lua_setfield(L, -2, "FUNC_DEBUG13");
  lua_pushinteger(L, FUNC_DEBUG14);
  lua_setfield(L, -2, "FUNC_DEBUG14");
  lua_pushinteger(L, FUNC_DEBUG15);
  lua_setfield(L, -2, "FUNC_DEBUG15");
  lua_pushinteger(L, FUNC_DEBUG16);
  lua_setfield(L, -2, "FUNC_DEBUG16");
  lua_pushinteger(L, FUNC_DEBUG17);
  lua_setfield(L, -2, "FUNC_DEBUG17");
  lua_pushinteger(L, FUNC_DEBUG18);
  lua_setfield(L, -2, "FUNC_DEBUG18");
  lua_pushinteger(L, FUNC_DEBUG19);
  lua_setfield(L, -2, "FUNC_DEBUG19");
  lua_pushinteger(L, FUNC_DEBUG20);
  lua_setfield(L, -2, "FUNC_DEBUG20");
  lua_pushinteger(L, FUNC_DEBUG21);
  lua_setfield(L, -2, "FUNC_DEBUG21");
  lua_pushinteger(L, FUNC_DEBUG22);
  lua_setfield(L, -2, "FUNC_DEBUG22");
  lua_pushinteger(L, FUNC_DEBUG23);
  lua_setfield(L, -2, "FUNC_DEBUG23");
  lua_pushinteger(L, FUNC_DEBUG24);
  lua_setfield(L, -2, "FUNC_DEBUG24");
  lua_pushinteger(L, FUNC_DEBUG25);
  lua_setfield(L, -2, "FUNC_DEBUG25");
  lua_pushinteger(L, FUNC_DEBUG26);
  lua_setfield(L, -2, "FUNC_DEBUG26");
  lua_pushinteger(L, FUNC_DEBUG27);
  lua_setfield(L, -2, "FUNC_DEBUG27");
  lua_pushinteger(L, FUNC_DEBUG28);
  lua_setfield(L, -2, "FUNC_DEBUG28");
  lua_pushinteger(L, FUNC_DEBUG29);
  lua_setfield(L, -2, "FUNC_DEBUG29");
  lua_pushinteger(L, FUNC_DEBUG30);
  lua_setfield(L, -2, "FUNC_DEBUG30");
  lua_pushinteger(L, FUNC_DEBUG31);
  lua_setfield(L, -2, "FUNC_DEBUG31");
  lua_pushinteger(L, FUNC_MAX);
  lua_setfield(L, -2, "FUNC_MAX");
  lua_pushinteger(L, FPIOA_PULL_NONE);
  lua_setfield(L, -2, "FPIOA_PULL_NONE");
  lua_pushinteger(L, FPIOA_PULL_DOWN);
  lua_setfield(L, -2, "FPIOA_PULL_DOWN");
  lua_pushinteger(L, FPIOA_PULL_UP);
  lua_setfield(L, -2, "FPIOA_PULL_UP");
  lua_pushinteger(L, FPIOA_PULL_MAX);
  lua_setfield(L, -2, "FPIOA_PULL_MAX");
  lua_pushinteger(L, FPIOA_DRIVING_0);
  lua_setfield(L, -2, "FPIOA_DRIVING_0");
  lua_pushinteger(L, FPIOA_DRIVING_1);
  lua_setfield(L, -2, "FPIOA_DRIVING_1");
  lua_pushinteger(L, FPIOA_DRIVING_2);
  lua_setfield(L, -2, "FPIOA_DRIVING_2");
  lua_pushinteger(L, FPIOA_DRIVING_3);
  lua_setfield(L, -2, "FPIOA_DRIVING_3");
  lua_pushinteger(L, FPIOA_DRIVING_4);
  lua_setfield(L, -2, "FPIOA_DRIVING_4");
  lua_pushinteger(L, FPIOA_DRIVING_5);
  lua_setfield(L, -2, "FPIOA_DRIVING_5");
  lua_pushinteger(L, FPIOA_DRIVING_6);
  lua_setfield(L, -2, "FPIOA_DRIVING_6");
  lua_pushinteger(L, FPIOA_DRIVING_7);
  lua_setfield(L, -2, "FPIOA_DRIVING_7");
  lua_pushinteger(L, FPIOA_DRIVING_8);
  lua_setfield(L, -2, "FPIOA_DRIVING_8");
  lua_pushinteger(L, FPIOA_DRIVING_9);
  lua_setfield(L, -2, "FPIOA_DRIVING_9");
  lua_pushinteger(L, FPIOA_DRIVING_10);
  lua_setfield(L, -2, "FPIOA_DRIVING_10");
  lua_pushinteger(L, FPIOA_DRIVING_11);
  lua_setfield(L, -2, "FPIOA_DRIVING_11");
  lua_pushinteger(L, FPIOA_DRIVING_12);
  lua_setfield(L, -2, "FPIOA_DRIVING_12");
  lua_pushinteger(L, FPIOA_DRIVING_13);
  lua_setfield(L, -2, "FPIOA_DRIVING_13");
  lua_pushinteger(L, FPIOA_DRIVING_14);
  lua_setfield(L, -2, "FPIOA_DRIVING_14");
  lua_pushinteger(L, FPIOA_DRIVING_15);
  lua_setfield(L, -2, "FPIOA_DRIVING_15");
  lua_pushinteger(L, FPIOA_DRIVING_MAX);
  lua_setfield(L, -2, "FPIOA_DRIVING_MAX");
  return 1;
}
