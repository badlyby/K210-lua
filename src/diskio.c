/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2016        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h" /* FatFs lower layer API */
#include "sdcard.h"
#include "w25qxx.h"
#include <stdio.h>

#define FLASH_OFF_SET_ADDRESS   (8*1024*1024)
#define FLASH_SPIFS_SIZE (w25qxx_FLASH_CHIP_SIZE - FLASH_OFF_SET_ADDRESS)

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/
static volatile int sdcard_ready = 0;
static volatile int spifs_ready = 0;
static uint8_t manuf_id = 0xFF, device_id = 0xFF;
void flash_init(uint8_t *manuf_id, uint8_t *device_id)
{
    w25qxx_init_dma(3, 0);
    w25qxx_enable_quad_mode_dma();
    w25qxx_read_id_dma(manuf_id, device_id);
}

DSTATUS disk_status(BYTE pdrv)
{
    switch(pdrv)
    {
    case 0:
        if(spifs_ready) return 0;
        break;
    case 1:
        if(sdcard_ready) return 0;
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize(BYTE pdrv)
{
    switch(pdrv)
    {
    case 0:
        flash_init(&manuf_id, &device_id);
        if((manuf_id != 0xFF) &&(device_id != 0xFF))
        {
            spifs_ready = 1;
            return 0;
        }
        break;
    case 1:
        if(sd_init() == 0)
        {
            sdcard_ready = 1;
            return 0;
        }
        break;
    }
    return STA_NOINIT;
}

/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read(BYTE pdrv, BYTE *buff, DWORD sector, UINT count)
{
    int ret = 0;
    switch(pdrv)
    {
    case 0:
        while(count--)
        {
            ret += w25qxx_read_data_dma(FLASH_OFF_SET_ADDRESS + w25qxx_FLASH_SECTOR_SIZE * sector, buff, w25qxx_FLASH_SECTOR_SIZE, W25QXX_QUAD_FAST);
            sector++;
            buff += w25qxx_FLASH_SECTOR_SIZE;
            if(ret != 0)
                return RES_ERROR;
        }
        break;
    case 1:
        if(sd_read_sector_dma(buff, sector, count) == 0)
            return RES_OK;
    }
    return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_write(BYTE pdrv, const BYTE *buff, DWORD sector, UINT count)
{
    int ret = 0;
    switch(pdrv)
    {
    case 0:
        while(count--)
        {
            ret += w25qxx_write_data_dma(FLASH_OFF_SET_ADDRESS + w25qxx_FLASH_SECTOR_SIZE * sector, buff, w25qxx_FLASH_SECTOR_SIZE);
            sector++;
            buff += w25qxx_FLASH_SECTOR_SIZE;
            if(ret != 0)
                return RES_ERROR;
        }
        break;
    case 1:
        if(sd_write_sector_dma((BYTE *)buff, sector, count) == 0)
            return RES_OK;
    }
    return RES_ERROR;
}

/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl(BYTE pdrv, BYTE cmd, void *buff)
{
    DRESULT res = RES_ERROR;
    switch(pdrv)
    {
    case 0:
        switch(cmd)
        {
            /* Make sure that no pending write process */
            case CTRL_SYNC:
                res = RES_OK;
                break;
            /* Get number of sectors on the disk (DWORD) */
            case GET_SECTOR_COUNT:
                *(DWORD *)buff = FLASH_SPIFS_SIZE / w25qxx_FLASH_SECTOR_SIZE;
                res = RES_OK;
                break;
            /* Get R/W sector size (WORD) */
            case GET_SECTOR_SIZE:
                *(WORD *)buff = w25qxx_FLASH_SECTOR_SIZE;
                res = RES_OK;
                break;
            /* Get erase block size in unit of sector (DWORD) */
            case GET_BLOCK_SIZE:
                *(DWORD *)buff = w25qxx_FLASH_PAGE_NUM_PER_SECTOR;
                res = RES_OK;
                break;
            default:
                res = RES_PARERR;
        }
        break;
    case 1:
        switch(cmd)
        {
            /* Make sure that no pending write process */
            case CTRL_SYNC:
                res = RES_OK;
                break;
            /* Get number of sectors on the disk (DWORD) */
            case GET_SECTOR_COUNT:
                *(DWORD *)buff = (cardinfo.SD_csd.DeviceSize + 1) << 10;
                res = RES_OK;
                break;
            /* Get R/W sector size (WORD) */
            case GET_SECTOR_SIZE:
                *(WORD *)buff = cardinfo.CardBlockSize;
                res = RES_OK;
                break;
            /* Get erase block size in unit of sector (DWORD) */
            case GET_BLOCK_SIZE:
                *(DWORD *)buff = cardinfo.CardBlockSize;
                res = RES_OK;
                break;
            default:
                res = RES_PARERR;
        }
        break;
    }
    return res;
}
