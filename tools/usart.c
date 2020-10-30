#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>

static struct termios gOriginalTTYAttrs;

void setDTR(int fuart, int set)
{
	int status;
	ioctl(fuart, TIOCMGET, &status);
	if(set) status |= TIOCM_DTR;
	else status &= ~TIOCM_DTR;
	ioctl(fuart, TIOCMSET, &status);
}

void setRTS(int fuart, int set)
{
	int status;
	ioctl(fuart, TIOCMGET, &status);
	if(set) status |= TIOCM_RTS;
	else status &= ~TIOCM_RTS;
	ioctl(fuart, TIOCMSET, &status);
}

int set_usart(const char *portname, int speed, int databits, int stopbits, int parity)
{
	int i,fuart;
	int status;
	int speed_arr[] = { 12000000, 6000000, 3000000, 2457600, 1228800, 921600, 806400, 614400, 460800, 403200, 268800, 256000, B230400, 201600, 161280, 134400, 128000, B115200, B57600, 56000, B38400, B28800, B19200, B14400, B9600, B7200, B4800, 3600, B2400, B1800, B1200, B600, B300, B150, B110, B75 };
	int name_arr[] = { 12000000, 6000000, 3000000, 2457600, 1228800, 921600, 806400, 614400, 460800, 403200, 268800, 256000, 230400, 201600, 161280, 134400, 128000, 115200, 57600, 56000, 38400, 28800, 19200, 14400, 9600, 7200, 4800, 3600, 2400, 1800, 1200, 600, 300, 150, 110, 75 };
	struct termios opt;
	fuart=open(portname,O_RDWR | O_NOCTTY | O_NDELAY);// | O_NONBLOCK);
	if(-1 == fuart) return -1;
	if(tcgetattr(fuart, &gOriginalTTYAttrs) != 0) return -1;
	opt = gOriginalTTYAttrs;
	for(i = 0; i<sizeof(speed_arr)/sizeof(int); i++)
	{
		if (speed==name_arr[i])
		{
			cfsetispeed(&opt, speed_arr[i]);
			cfsetospeed(&opt, speed_arr[i]);
		}
	}
	opt.c_cflag &= ~CSIZE;
	opt.c_lflag &= ~( ISIG | ICANON | ECHO | ECHOE | ECHOK | ECHONL | NOFLSH | IEXTEN | ECHOCTL | ECHOPRT| ECHOKE| FLUSHO | PENDIN | TOSTOP );
	//opt.c_lflag |= NOFLSH;
	opt.c_oflag &= ~OPOST;
	opt.c_cflag &= ~PARENB;
	opt.c_iflag &= ~INPCK;
	opt.c_cflag &= ~PARODD;
	opt.c_cflag &= ~CSTOPB;
	switch (databits)
	{
	case 5:
		opt.c_cflag |= CS5;
		break;
	case 6:
		opt.c_cflag |= CS6;
		break;
	case 7:
		opt.c_cflag |= CS7;
		break;
	case 8:
		opt.c_cflag |= CS8;
		break;
	default:
		return(-1);
	}
	switch (parity)
	{
	case 'n':
	case 'N':
		break;
	case 'o':
	case 'O':
		opt.c_cflag |= (PARODD | PARENB);
		opt.c_iflag |= INPCK;
		break;
	case 'e':
	case 'E':
		opt.c_cflag |= PARENB;
		opt.c_iflag |= INPCK;
		break;
	case 'S':
	case 's':
		opt.c_iflag |= INPCK;
		break;
	default:
		return(-1);
	}
	if(stopbits == 2) opt.c_cflag |= CSTOPB;
	opt.c_iflag |= IGNBRK;
	opt.c_iflag |= IGNPAR;
	opt.c_iflag &=~(PARMRK | ISTRIP | IGNCR | IXON | IXOFF | IXANY | BRKINT | INLCR | ICRNL | IMAXBEL);
	opt.c_oflag &=~(OPOST | ONLCR | ONLRET | OFILL | OFDEL | NLDLY | NL1 | CRDLY | CR1 | CR2 | CR3 | TABDLY | TAB1 | TAB2 | TAB3 | BSDLY | BS1 | VTDLY | VT1 | FFDLY | FF1);
	opt.c_oflag |= (ONOCR | NL0 | CR0 | TAB0 | BS0 | VT0 | FF0);
	opt.c_cc[VTIME] = 1;
	opt.c_cc[VMIN] = 1;
	status = tcsetattr(fuart, TCSANOW, &opt);
	if (status == 0)
	{
		tcflush(fuart, TCIOFLUSH);
		return fuart;
	}
	if(-1 != fuart) close(fuart);
    return -1;
}

void usart_close(int fuart)
{
	if (tcdrain(fuart) == -1) {
        printf("Error waiting for drain - %s(%d).\n",
               strerror(errno), errno);
    }
    if (tcsetattr(fuart, TCSANOW, &gOriginalTTYAttrs) == -1) {
        printf("Error resetting tty attributes - %s(%d).\n",
               strerror(errno), errno);
    }
    close(fuart);
}
