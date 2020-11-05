/*
The MIT License (MIT)

Copyright (c) 2020 badlyby

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include "loadbmp.h"

int read_bmp_size(const char *name, uint32_t *width, uint32_t *height)
{
    FIL fp;
    UINT rn;
	uint32_t head;
    if(f_open(&fp, name,FA_READ) != FR_OK)
    {
        printf("Can not open file %s to read!\n",name);
        return -1;
    }
    f_read(&fp, &head,4,&rn);
    if((head & 0x0000ffff) != 0x00004D42)
    {
        printf("File is not a bmp file!\n");
        f_close(&fp);
        return -1;
    }
    f_lseek(&fp, 18);
    f_read(&fp, width, 4, &rn);
    f_lseek(&fp, 22);
    f_read(&fp, height, 4, &rn);
    f_close(&fp);
    return 0;
}

int loadbmp(const char *name, uint16_t *imgbuf)
{
    FIL fp;
    UINT rn;
    int length=0,i,j;
	uint32_t head,width,height,offset,wid;
	uint32_t bitcount;
	uint8_t *readbuf = NULL;
	uint16_t *colorpan = NULL;
	uint8_t *pbuf;
	int pancount = 0;
    if(f_open(&fp, name,FA_READ) != FR_OK)
    {
        printf("Can not open file %s to read!\n",name);
        return -1;
    }
    f_read(&fp, &head,4,&rn);
    if((head & 0x0000ffff) != 0x00004D42)
    {
        printf("File is not a bmp file!\n");
        f_close(&fp);
        return -1;
    }
    f_lseek(&fp, 10);
    f_read(&fp, &offset, 4, &rn);
    f_lseek(&fp, 18);
    f_read(&fp, &width, 4, &rn);
    f_lseek(&fp, 22);
    f_read(&fp, &height, 4, &rn);
    f_lseek(&fp, 28);
    f_read(&fp, &bitcount, 4, &rn);
    if((width>320)||(height>240))
    {
        printf("Size is too big!");
        f_close(&fp);
        return -1;
    }
    if(bitcount < 16)
    {
        pancount = 1<<bitcount;
        colorpan = malloc(pancount * 2);
        pbuf = malloc(pancount * 4);
        f_lseek(&fp,54);
        f_read(&fp, pbuf, pancount*4, &rn);
        for(i=0;i<pancount;i++)
        {
            colorpan[i] = RGB565(pbuf[i*4+2], pbuf[i*4+1], pbuf[i*4]);
        }
        free(pbuf);
    }
    else
        pancount  = 0;
    wid = (width*bitcount/8+3)/4*4;
    length = wid*height;
    readbuf = (unsigned char*) malloc(length);
    f_lseek(&fp, offset);
    f_read(&fp, readbuf, length, &rn);
    f_close(&fp);
    if(rn != length)
    {
        printf("Read file size error!\n");
        free(readbuf);
        if(pancount)
            free(colorpan);
        return -1;
    }
    /*if(bitcount == 1)
    {
        for(i=0;i<height;i++)
        {
            for(j=0;j<wid*8;j++)
            {
                if(j<((width+7)/8*8))
                {
                    if(readbuf[i*wid+j/8] & (1<<(j%8))) imgbuf[(height-1-i)*width+j] = 0xFFFF;
                    else imgbuf[(height-1-i)*width+j] = 0;
                }
            }
        }
    }*/
    if(bitcount == 8)
    {
        for(i=0;i<height;i++)
        {
            for(j=0;j<wid;j++)
            {
                if(j<((width+7)/8*8))
                {
                    imgbuf[(height-1-i)*width+j] = colorpan[readbuf[i*wid+j]];
                }
            }
        }
    }
    if(bitcount == 16)
    {
        for(i=0;i<height;i++)
        {
            for(j=0;j<wid/2;j++)
            {
                if(j<width)
                {
                    imgbuf[(height-1-i)*width+j] = ((uint16_t *)readbuf)[i*(wid/2)+j];
                }
            }
        }
    }
    if(bitcount == 24)
    {
        for(i=0;i<height;i++)
        {
            for(j=0;j<wid/3;j++)
            {
                if(j<width)
                {
                    imgbuf[(height-1-i)*width+j] = RGB565(readbuf[i*wid+j*3+2], readbuf[i*wid+j*3+1], readbuf[i*wid+j*3]);
                }
            }
        }
    }
    free(readbuf);
    if(pancount)
        free(colorpan);
    return 0;
}
