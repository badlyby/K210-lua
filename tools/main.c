#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>
#include <termios.h>
#include "usart.h"
#include "SPE_NL.h"

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

static int fusart = -1;
volatile int SPE_ACK = 0;
void SPE_CallBack(uint8_t *data, uint32_t length)
{
	uint8_t cmd =data[0];
	uint16_t *v16;
	uint32_t *v32;
	uint64_t *v64;
	SPE_ACK = 1;
	switch(cmd)
	{
	case SPE_RETURN_U8:
		printf("Return U8 %d\n", data[1]);
		break;
	case SPE_RETURN_U16:
		v16 = (uint16_t *)(data+1);
		printf("Return U16 %d\n", *v16);
		break;
	case SPE_RETURN_U32:
		v32 = (uint32_t *)(data+1);
		printf("Return U32 %d\n", *v32);
		break;
	case SPE_RETURN_U64:
		v64 = (uint64_t *)(data+1);
		printf("Return U64 %ld\n", (unsigned long)*v64);
		break;
	}
}

void SendBytes(uint8_t *bytes, uint32_t length)
{
	write(fusart, bytes, length);
}

int find_name(char *path)
{
	int i,len,ret = -1;
	len = strlen(path);
	for(i=0;i<len;i++)
	{
		if((path[i] == '\\') || (path[i] == '/'))
			ret = i+1;
	}
	return ret;
}

int main(int argc, char *argv[])
{
	int i,rlen,len,timeout = 500;
	uint8_t buf[9000];
	char *filename = NULL;
	FILE *fp = NULL;
	if(argc != 3)
	{
		printf("%s serialport filename\n", argv[0]);
		return -1;
	}
	printf("Open %s.\n",argv[1]);
	i = find_name(argv[2]);
	if(i>0) filename = argv[2]+i;
	else filename = argv[2];
	printf("File %s\n",filename);
	fusart = set_usart(argv[1], 115200, 8, 1, 'N');
	if(-1 == fusart)
	{
		printf("Can't open serial port %s\n",argv[1]);
		return -2;
	}
	fp = fopen(argv[2], "rb");
	if(fp == NULL)
	{
		printf("Can't open file %s\n",argv[2]);
		usart_close(fusart);
		return -3;
	}
	setDTR(fusart, 1);
	setRTS(fusart, 1);
	sprintf((char *)buf, "into_spe()\n");
	write(fusart, buf, strlen((char *)buf));
	usleep(10*1000);
	len = read(fusart,buf,256);
	if(len < 256)
	{
		buf[len] = 0;
		printf("%s\n",buf);
	}
	buf[0] = SPE_CMD_OFW;
	sprintf((char *)(buf+1),"0:/%s",filename);
	SPE_ACK = 0;
	SPE_Send_Packet(buf, strlen((char *)buf));
	timeout = 5000;
	while(SPE_ACK == 0)
	{
		len = read(fusart,buf,9000);
		for(i=0;i<len;i++)
		{
			SPE_Receive_Byte(buf[i]);
		}
		usleep(1000);
		if(timeout)
			timeout--;
		else
			break;
	}

	buf[0] = SPE_CMD_WF;
	do {
		rlen = fread(buf+1, 1, 8192, fp);
		if(rlen <= 0)
			break;
		SPE_ACK = 0;
		SPE_Send_Packet(buf, rlen+1);
		timeout = 5000;
		while(SPE_ACK == 0)
		{
			len = read(fusart,buf,9000);
			for(i=0;i<len;i++)
			{
				SPE_Receive_Byte(buf[i]);
			}
			usleep(1000);
			if(timeout)
				timeout--;
			else
				break;
		}
	}while(rlen == 8192);
	fclose(fp);
	buf[0] = SPE_CMD_CF;
	SPE_ACK = 0;
	SPE_Send_Packet(buf, 1);
	timeout = 5000;
	while(SPE_ACK == 0)
	{
		len = read(fusart,buf,9000);
		for(i=0;i<len;i++)
		{
			SPE_Receive_Byte(buf[i]);
		}
		usleep(1000);
		if(timeout)
			timeout--;
		else
			break;
	}

	/*buf[0] = SPE_CMD_DF;
	sprintf(buf+1,"0:/%s", filename);
	SPE_Send_Packet(buf, strlen(buf));*/
	buf[0] = SPE_CMD_EXIT;
	SPE_Send_Packet(buf, 1);
	/*usleep(100*1000);
	sprintf(buf, "dofile(\"0:/test.lua\")\n");
	write(fusart, buf, strlen(buf));*/
	/*usleep(100*1000);
	len = read(fusart,buf,256);
	if(len < 256)
	{
		buf[len] = 0;
		printf("%s\n",buf);
	}*/
	usart_close(fusart);
	return 0;
}