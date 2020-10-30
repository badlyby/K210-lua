#ifndef __WZL_USART_H__
	#define __WZL_USART_H__
	void setDTR(int fuart, int set);
	void setRTS(int fuart, int set);
	int set_usart(const char *portname, int speed, int databits, int stopbits, int parity);
	void usart_close(int fuart);
#endif