#include <stdio.h>
#include <gpio.h>
#include <sleep.h>
#include <fpioa.h>
#include <bsp.h>
#include <dmac.h>
#include <ff.h>
#include <plic.h>
#include <sdcard.h>
#include <sysctl.h>
#include <uarths.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

LUAMOD_API int luaopen_fpioa (lua_State *L);
LUAMOD_API int luaopen_gpio (lua_State *L);

static int sdcard_init(void)
{
    uint8_t status;
    status = sd_init();
    if(status) printf("sd init %d\n", status);
    if(status != 0)
    {
        return status;
    }
    printf("CardCapacity:%ld\n", cardinfo.CardCapacity);
    printf("CardBlockSize:%d\n", cardinfo.CardBlockSize);
    return 0;
}

static int fs_init(void)
{
    static FATFS sdcard_fs;
    FRESULT status;
    status = f_mount(&sdcard_fs, _T("0:"), 1);
    if(status) printf("mount sdcard:%d\n", status);
    if(status != FR_OK)
        return status;
    return 0;
}

static int lua_current_coreid(lua_State *L)
{
    uint64_t core = current_coreid();
    lua_pushinteger(L, core);
    return 1;
}

int run_on_core1(void *ctx)
{
    lua_State *L = ctx;
    int n = lua_gettop(L);
    lua_call(L, n, 0);
    while(1);
    return 0;
}

static int lua_calloncore1(lua_State *L)
{
    //register_core1(run_on_core1, L);
    int n = lua_gettop(L);
    lua_call(L, n, 0);
    return 0;
}

int main()
{
    int status;
    sysctl_pll_set_freq(SYSCTL_PLL0, 800000000UL);
    plic_init();
    sysctl_enable_irq();
    fpioa_init();
    fpioa_set_function(28, FUNC_SPI0_D0);
    fpioa_set_function(26, FUNC_SPI0_D1);
    fpioa_set_function(27, FUNC_SPI0_SCLK);
    fpioa_set_function(29, FUNC_GPIOHS7);
    fpioa_set_function(25, FUNC_SPI0_SS3);
    if(sdcard_init())
    {
        printf("SD card err\n");
        return -1;
    }
    if(fs_init())
    {
        printf("FAT32 err\n");
        return -1;
    }
    lua_State *L = luaL_newstate();  /* create state */
    luaL_openlibs(L);
    luaL_requiref(L, "fpioa",luaopen_fpioa, 1);
    luaL_requiref(L, "gpio",luaopen_gpio, 1);
    lua_register(L, "current_coreid", lua_current_coreid);
    lua_register(L, "do_core1",lua_calloncore1);
    status=luaL_dofile(L, "main.lua");if(status) printf("error\n");
    while(1);
    lua_close(L);
}
