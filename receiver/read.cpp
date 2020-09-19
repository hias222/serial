#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define COLORADO_ADDRESS_WORD_MASK 0x80
#define BUFFER_LENGTH 32

#include <string.h>
#include <string>

#define debug_incoming

// Linux headers
#include <errno.h> // Error integer and strerror() function
#include <unistd.h>
#include "analyseData.h"

#define BAUDRATE B38400
//#define MODEMDEVICE "/dev/ttys002"
//#define MODEMDEVICE "/dev/ttyUSB0"
//#define MODEMDEVICE "/dev/cu.usbserial-14610"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1
#define info_read

volatile int STOP = FALSE;

using namespace std;

int read(char *portname, volatile int *running)
{
    int fd, c, res;
    struct termios oldtio, newtio;
    unsigned char buf[BUFFER_LENGTH];

#ifdef info_read
    int outputnr;
    outputnr = 9999;
#endif

    fd = open(portname, O_RDONLY);
    if (fd < 0)
    {
        perror(portname);
        return 1;
    }

    tcgetattr(fd, &oldtio); //save current port settings 
    bzero(&newtio, sizeof(newtio));

    /*

    newtio.c_cflag = ~CRTSCTS & ~PARENB & ~CSTOPB;
    newtio.c_cflag |= CS8 | CLOCAL | CREAD;
    cfsetispeed(&newtio, B4800);
    cfsetospeed(&newtio, B4800);

    //newtio.c_cflag = BAUDRATE | ~CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    // set input mode (non-canonical, no echo,...) 
    newtio.c_lflag &= ~ICANON; // 0

    */
    // new
    
    newtio.c_cflag = (newtio.c_cflag & ~CSIZE) | CS8;
	
	//set into raw, no echo mode
	newtio.c_iflag = IGNBRK;
	newtio.c_lflag = 0;
	newtio.c_oflag = 0;
	newtio.c_cflag |= CLOCAL | CREAD;

	newtio.c_cflag &= ~CRTSCTS;
	newtio.c_cc[VMIN] = 1;
	newtio.c_cc[VTIME] = 5;

	//turn off software control
	newtio.c_iflag &= ~(IXON | IXOFF | IXANY);

	//no parity
	newtio.c_cflag &= ~(PARENB | PARODD);

	//1 stopbit
	newtio.c_cflag &= ~CSTOPB;
    
    newtio.c_cc[VTIME] = 0; // inter-character timer unused 
    newtio.c_cc[VMIN] = 5;  // blocking read until 5 chars received 

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    printf("\n");
    printf("    port in (USB)= %s\n", portname);
    printf("\n");

    int g = 0;

    while (*running)
    {
        res = read(fd, buf, BUFFER_LENGTH); /* returns after 5 chars have been input */
        buf[res] = 0;

#ifdef info_read
        if (outputnr > 2048)
        {
            outputnr = 0;
            time_t now;
            time(&now);
            printf("%s getting 2048 bytes", ctime(&now));
            printf("\n");
        }
#endif

        for (int i = 0; i < res; i++)
        {

#ifdef info_read
            outputnr++;
#endif

#ifdef debug_incoming
            // printf("%d: %02x ", g, buf[i]);
            printf("%02x ",buf[i]);
#endif

            if ((buf[i] & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
            {
#ifdef debug_incoming
                printf("\n %02x \n", COLORADO_ADDRESS_WORD_MASK);
#endif
                g = 0;
            }

            putReadData(buf[i]);
            g++;
        }
        if (!*running)
        {
            printf("\n\nexit\n");
        }
        fflush(stdout);
    }
    tcsetattr(fd, TCSANOW, &oldtio);
    return 1;
}
