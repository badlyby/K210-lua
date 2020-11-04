#include <stdio.h>
#include <stdint.h>
#include <gpio.h>
#include <sleep.h>
#include <fpioa.h>
#include <bsp.h>
#include <dmac.h>
#include <ff.h>
#include <plic.h>
#include <diskio.h>
#include <sysctl.h>
#include <uarths.h>
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "lstate.h"
#include "w25qxx.h"
#include "sdcard.h"
#include "SPE_NL.h"
#include "wzl-fifo.h"
#include <uart.h>
#include "lcd.h"

#define SPE_CMD_NOP 0
#define SPE_CMD_OFW 1 //open file for write
#define SPE_CMD_OFR 2 //open file for read
#define SPE_CMD_WF  3 //write file
#define SPE_CMD_RF  4 //read file
#define SPE_CMD_CF  5 //close file
#define SPE_CMD_DF  6 //do file
#define SPE_RETURN_U8   0x80
#define SPE_RETURN_U16  0x81
#define SPE_RETURN_U32  0x82
#define SPE_RETURN_U64  0x83
#define SPE_ERROR   0xFE
#define SPE_CMD_EXIT 0xFF //exit spe mode

static uint8_t spe_retbuf[9000];
WZL_FIFO spe_fifo;
uint8_t spe_fifo_buf[9000];

volatile int running_spe = 0;

LUAMOD_API int luaopen_fpioa (lua_State *L);
LUAMOD_API int luaopen_gpio (lua_State *L);
LUAMOD_API int luaopen_uart (lua_State *L);
LUAMOD_API int luaopen_spi (lua_State *L);
LUAMOD_API int luaopen_lcd (lua_State *L);
LUAMOD_API int luaopen_image (lua_State *L);
int dofile (lua_State *L, const char *name);
void into_main(lua_State *L);
static lua_State *L, *L1;
static volatile int core1_busy_flag = 0;

static int lua_current_coreid(lua_State *L)
{
    uint64_t core = current_coreid();
    lua_pushinteger(L, core);
    return 1;
}

static int lua_core1_busy(lua_State *L)
{
    lua_pushboolean(L,atomic_read(&core1_busy_flag));
    return 1;
}

static int lua_core1_free(lua_State *L)
{
    lua_resetthread(L1);
    atomic_set(&core1_busy_flag, 0);
    return 0;
}

int run_on_core1(void *ctx)
{
    int n;
    while(1)
    {
        while(atomic_read(&core1_busy_flag) == 0) ;
        n = lua_gettop(L1);
        lua_pcall(L1, n-1, 0, 0);
        atomic_set(&core1_busy_flag, 0);
    }
    return 0;
}

static int lua_do_core1(lua_State *L)
{
    int i,n;
    if(atomic_read(&core1_busy_flag)) 
    {
        lua_pushstring(L, "core1 busy");
        return 1;
    }
    n =lua_gettop(L);
    lua_settop(L1,n);
    for(i=1;i<=n;i++)
        lua_copy2(L,i,L1,i);
    atomic_set(&core1_busy_flag, 1);
    return 0;
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

void make_main_lua(void)
{
    FIL file;
    const char lua_str[] = "print \"hello spifs!\"";
    uint32_t v_ret_len = 0;
    if(f_open(&file, "0:/main.lua", FA_READ) != FR_OK)
    {
        printf("make main.lua\n");
        if(f_open(&file, "0:/main.lua", FA_CREATE_ALWAYS | FA_WRITE) == FR_OK)
        {
            f_write(&file, lua_str, sizeof(lua_str)-1, &v_ret_len);
            f_close(&file);
        }
    }
    else
    {
        f_close(&file);
    }
}

static int fs_init(void)
{
    static FATFS sdcard_fs,spi_fs;
    BYTE *buf;
    FRESULT status;
    status = f_mount(&spi_fs, _T("0:"), 1);
    if(status == FR_NO_FILESYSTEM)
    {
        buf = malloc(64*1024);
        status = f_mkfs(_T("0:"), FM_ANY, 0, buf, 64*1024);
        free(buf);
        if(status == FR_OK)
        {
            status = f_mount(&spi_fs, _T("0:"), 1);
        }
    }
    if(status) printf("mount spifs:%d %d\n", status,disk_status(0));
    if(status == 0) printf("spifs mounted\n");
    status = f_mount(&sdcard_fs, _T("1:"), 1);
    if(status) printf("mount sdcard:%d\n", status);
    if(status != FR_OK)
        return status;
    return 0;
}

static int lua_sys_reset(lua_State *L)
{
    sysctl->soft_reset.soft_reset = 1;
    return 0;
}

int spe_get_bytes(void *ctx)
{
    uint8_t buf[14],i,len;
    len = uart_receive_data(UART_DEVICE_3, (char *)buf, 14);
    for(i=0;i<len;i++)
        FIFO_push(&spe_fifo, buf[i]);
    return 0;
}

static int lua_into_spe(lua_State *L)
{
    running_spe = 1;
    int i,len;
    uint8_t ch = 0;
    fflush(stdin);
    fflush(stdout);
    uart_irq_register(UART_DEVICE_3, UART_RECEIVE, spe_get_bytes, NULL, 1);
    spe_retbuf[0] = SPE_RETURN_U8;
    spe_retbuf[1] = 0;
    SPE_Send_Packet(spe_retbuf, 2);
    while(running_spe)
    {
        len = FIFO_getSize(&spe_fifo);
        for(i=0;i<len;i++)
        {
            FIFO_get(&spe_fifo, &ch);
            SPE_Receive_Byte(ch);
        }
    }
    printf("Leaving spe.\r\n");
    return 0;
}

FIL spe_file;
void SPE_CallBack(uint8_t *data, uint32_t length)
{
    uint8_t cmd = 0;
    UINT v_ret_len;
    if(length)
    {
        cmd = data[0];
        switch(cmd)
        {
        case SPE_CMD_OFW:
            if((length >= 2) && (length < 9000))
            {
                data[length] = 0;
                spe_retbuf[0] = SPE_RETURN_U8;
                spe_retbuf[1] = f_open(&spe_file, (const char *)(data+1), FA_WRITE | FA_CREATE_ALWAYS);
                SPE_Send_Packet(spe_retbuf, 2);
            }
            break;
        case SPE_CMD_OFR:
            if((length >= 2) && (length < 9000))
            {
                data[length] = 0;
                spe_retbuf[0] = SPE_RETURN_U8;
                spe_retbuf[1] = f_open(&spe_file, (const char *)(data+1), FA_READ);
                SPE_Send_Packet(spe_retbuf, 2);
            }
            break;
        case SPE_CMD_WF:
            if(length > 1)
            {
                spe_retbuf[0] = SPE_RETURN_U8;
                spe_retbuf[1] = f_write(&spe_file, data+1, length-1, &v_ret_len);
                SPE_Send_Packet(spe_retbuf, 2);
            }
            break;
        case SPE_CMD_RF:
            spe_retbuf[0] = f_read(&spe_file, spe_retbuf+1, 8192, &v_ret_len);
            SPE_Send_Packet(spe_retbuf, v_ret_len+1);
            break;
        case SPE_CMD_CF:
            spe_retbuf[0] = SPE_RETURN_U8;
            spe_retbuf[1] = f_close(&spe_file);
            SPE_Send_Packet(spe_retbuf, 2);
            break;
        case SPE_CMD_DF:
            if((length >= 2) && (length < 9000))
            {
                data[length] = 0;
                uart_irq_unregister(UART_DEVICE_3, UART_RECEIVE);
                running_spe = 0;
                dofile(L, (const char *)(data+1));
            }
            break;
        case SPE_CMD_EXIT:
            uart_irq_unregister(UART_DEVICE_3, UART_RECEIVE);
            running_spe = 0;
            break;
        }
    }
}

void SendBytes(uint8_t *bytes, uint32_t length)
{
    fwrite(bytes, 1, length, stdout);
    fflush(stdout);
}

void SPE_CRCError(uint16_t s,uint16_t d)
{
    spe_retbuf[0] = SPE_ERROR;
    SPE_Send_Packet(spe_retbuf, 1);
}

static int lua_read_cycle(lua_State *L)
{
    lua_pushinteger(L, read_cycle());
    return 1;
}

static int lua_cpu_freq(lua_State *L)
{
    lua_pushinteger(L, sysctl_clock_get_freq(SYSCTL_CLOCK_CPU));
    return 1;
}

void fpioa_config(void)
{
    fpioa_init();
    //sdcard
    fpioa_set_function(28, FUNC_SPI0_D0);
    fpioa_set_function(26, FUNC_SPI0_D1);
    fpioa_set_function(27, FUNC_SPI0_SCLK);
    fpioa_set_function(29, FUNC_GPIOHS0 + SDCARD_SELECT);
    fpioa_set_function(25, FUNC_SPI0_SS0 + SPI_DEVICE_SS);
    //lcd
    fpioa_set_function(37, FUNC_GPIOHS0 + RST_GPIONUM);
    fpioa_set_function(38, FUNC_GPIOHS0 + DCX_GPIONUM);
    fpioa_set_function(36, FUNC_SPI0_SS0 + LCD_SPI_SLAVE_SELECT);
    fpioa_set_function(39, FUNC_SPI0_SCLK);
}

int main()
{
    FIFO_Init(&spe_fifo, spe_fifo_buf);
    sysctl_cpu_set_freq(200000000UL);
    sysctl_set_power_mode(SYSCTL_POWER_BANK6, SYSCTL_POWER_V18);
	sysctl_set_power_mode(SYSCTL_POWER_BANK7, SYSCTL_POWER_V18);
    plic_init();
    sysctl_enable_irq();
    dmac_init();
    fpioa_config();
    printf("CPU Freq %d\n", sysctl_cpu_get_freq());
    if(fs_init())
    {
        printf("FAT32 err\n");
    }
    make_main_lua();
    L = luaL_newstate();  /* create state */
    luaL_openlibs(L);
    luaL_requiref(L, "fpioa",luaopen_fpioa, 1);
    luaL_requiref(L, "gpio",luaopen_gpio, 1);
    luaL_requiref(L, "uart",luaopen_uart, 1);
    luaL_requiref(L, "spi",luaopen_spi, 1);
    luaL_requiref(L, "lcd",luaopen_lcd, 1);
    luaL_requiref(L, "image",luaopen_image, 1);
    lua_register(L, "usleep", lua_usleep);
    lua_register(L, "msleep", lua_msleep);
    lua_register(L, "sleep", lua_sleep);
    lua_register(L, "sys_reset", lua_sys_reset);
    lua_register(L, "read_cycle", lua_read_cycle);
    lua_register(L, "cpu_freq", lua_cpu_freq);
    lua_register(L, "current_coreid", lua_current_coreid);
    lua_register(L, "do_core1", lua_do_core1);
    lua_register(L, "core1_busy", lua_core1_busy);
    lua_register(L, "core1_free", lua_core1_free);
    lua_register(L, "into_spe", lua_into_spe);
    L1 = lua_newthread(L);
    register_core1(run_on_core1, NULL);
    into_main(L);
    while(1);
    luaE_freethread(L, L1);
    lua_close(L);
}
