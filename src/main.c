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
#include "lstate.h"

LUAMOD_API int luaopen_fpioa (lua_State *L);
LUAMOD_API int luaopen_gpio (lua_State *L);
lua_State *L, *L1;
static volatile int  core1_busy_flag = 0;

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

static int lua_core1_busy(lua_State *L)
{
    lua_pushinteger(L, core1_busy_flag);
    return 1;
}

static int lua_core1_free(lua_State *L)
{
    luaE_freethread(L, L1);
    core1_busy_flag = 0;
    return 0;
}

int run_on_core1(void *ctx)
{
    int n = lua_gettop(L1);
    lua_pcall(L1, n-1, 0, 0);
    luaE_freethread(L, L1);
    core1_busy_flag = 0;
    return 0;
}

static int lua_do_core1(lua_State *L)
{
    int i,n,ret = 0;
    if(core1_busy_flag) 
    {
        lua_pushstring(L, "core1 busy");
        return 1;
    }
    L1 = lua_newthread(L);
    //lua_getglobal(L1, "print");
    n =lua_gettop(L);
    lua_getglobal(L1, "on_core1");
    lua_settop(L1,n+1);
    for(i=1;i<n;i++)
    lua_copy2(L,i,L1,i+1);
    core1_busy_flag = 1;
    ret = register_core1(run_on_core1, L);
    lua_pushinteger(L, ret);
    return 1;
}

static int lua_usleep(lua_State *L)
{
    usleep(luaL_checkinteger(L, 1));
    return 0;
}

static int lua_msleep(lua_State *L)
{
    msleep(luaL_checkinteger(L, 1));
    return 0;
}

static int lua_sleep(lua_State *L)
{
    sleep(luaL_checkinteger(L, 1));
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
    L = luaL_newstate();  /* create state */
    luaL_openlibs(L);
    luaL_requiref(L, "fpioa",luaopen_fpioa, 1);
    luaL_requiref(L, "gpio",luaopen_gpio, 1);
    lua_register(L, "usleep", lua_usleep);
    lua_register(L, "msleep", lua_msleep);
    lua_register(L, "sleep", lua_sleep);
    lua_register(L, "current_coreid", lua_current_coreid);
    lua_register(L, "do_core1", lua_do_core1);
    lua_register(L, "core1_busy", lua_core1_busy);
    lua_register(L, "core1_free", lua_core1_free);
    status=luaL_dofile(L, "main.lua");if(status) printf("error\n");
    while(1);
    lua_close(L);
}
