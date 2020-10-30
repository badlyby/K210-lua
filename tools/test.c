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

void SPE_CallBack(uint8_t *data, uint32_t length)
{
	//
}

void SendBytes(uint8_t *bytes, uint32_t length)
{
	write(fusart, bytes, length);
}

int main(int argc, char *argv[])
{
	int len;
	char buf[256];
	char cmd[] = "into_spe()\r\n";
	char test[]="print \"hello spe\"";
	if(argc < 2)
		return -1;
	printf("Open %s.\n",argv[1]);
	fusart = set_usart(argv[1], 115200, 8, 1, 'N');
	if(-1 == fusart)
		return -2;
	setDTR(fusart, 1);
	setRTS(fusart, 1);
	write(fusart, cmd, strlen(cmd));
	usleep(10000);
	buf[0] = SPE_CMD_OFW;
	sprintf(buf+1,"0:/test.lua");
	SPE_Send_Packet(buf, strlen(buf));
	usleep(10000);
	buf[0] = SPE_CMD_WF;
	sprintf(buf+1,"%s",test);
	SPE_Send_Packet(buf, strlen(buf));
	usleep(10000);
	buf[0] = SPE_CMD_CF;
	SPE_Send_Packet(buf, 1);
	usleep(10000);
	buf[0] = SPE_CMD_DF;
	sprintf(buf+1,"0:/test.lua");
	SPE_Send_Packet(buf, strlen(buf));
	//buf[0] = SPE_CMD_EXIT;
	//SPE_Send_Packet(buf, 1);
	usleep(100*1000);
	len = read(fusart,buf,256);
	if(len < 256)
	{
		buf[len] = 0;
		printf("%s\n",buf);
	}
	usart_close(fusart);
	return 0;
}