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

void SPE_CallBack(uint8_t *data, uint32_t length)
{
	//
}

void SendBytes(uint8_t *bytes, uint32_t length)
{
	//
}

static int fusart = -1;
int main(int argc, char *argv[])
{
	int len;
	char buf[256];
	if(argc < 2)
		return -1;
	printf("Open %s.\n",argv[1]);
	fusart = set_usart(argv[1], 115200, 8, 1, 'N');
	if(-1 == fusart)
		return -2;
	setDTR(fusart, 1);
	setRTS(fusart, 1);
	write(fusart, "print(\"hello test\")\r\n", 22);
	usleep(1000*1000);
	len = read(fusart, buf, 256);
	if(len < 256)
	{
		buf[len] = 0;
		printf("%s\n", buf);
	}
	usart_close(fusart);
	return 0;
}