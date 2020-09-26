#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define COLORADO_ADDRESS_WORD_MASK 0x80
#define COLORADO_ERROR_CR 0x35
#define BUFFER_LENGTH 32

#include <string.h>
#include <string>



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

//  debug
#define info_read
#define debug_incoming

volatile int STOP = FALSE;

using namespace std;

int read(char *portname, volatile int *running)
{
    int fd, c, res;
    int flag, baudrate;
    struct termios oldtio, newtio;
    unsigned char buf[BUFFER_LENGTH];

#ifdef info_read
    int outputnr;
    outputnr = 9999;
#endif

    //fd = open(portname, O_RDONLY);
    fd = open(portname, O_RDWR | O_NOCTTY | O_NDELAY);

    
    if (fd < 0)
    {
        perror(portname);
        return 1;
    }

    tcgetattr(fd, &oldtio); //save current port settings 
    bzero(&newtio, sizeof(newtio));

    // new
    // https://www.linuxquestions.org/questions/programming-9/why-minicom-reads-data-correctly-from-dev-ttyusb0-but-my-program-does-not-904382/
    
    flag = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flag & ~O_NDELAY);

    //set baudrate to 0 and go back to normal
	printf("set baudrate to 0......\n");
	tcgetattr(fd, &newtio);
	tcgetattr(fd, &oldtio);
	cfsetospeed(&newtio, B0);
	cfsetispeed(&newtio, B0);
	tcsetattr(fd, TCSANOW, &newtio);
	sleep(1);
	tcsetattr(fd, TCSANOW, &oldtio);
	printf("baudrate is back to normal......\n");

	tcgetattr(fd, &newtio);

	baudrate = B9600;
	cfsetospeed(&newtio, baudrate);
	cfsetispeed(&newtio, baudrate);

    newtio.c_cflag = (newtio.c_cflag & ~CSIZE) | CS8;
	
	//set into raw, no echo mode
	newtio.c_iflag = ~IGNBRK;
    newtio.c_iflag &= BRKINT;
    //newtio.c_iflag = ICRNL;

	newtio.c_lflag = 0;
	newtio.c_oflag = 0;
	newtio.c_cflag |= CLOCAL | CREAD;

	newtio.c_cflag &= ~CRTSCTS;

	//turn off software control
	newtio.c_iflag &= ~(IXON | IXOFF | IXANY);

	//no parity
	newtio.c_cflag &= ~(PARENB | PARODD);

	//1 stopbit
	newtio.c_cflag &= ~CSTOPB;
    
    newtio.c_cc[VTIME] = 1; // inter-character timer unused 
    newtio.c_cc[VMIN] = 0;  // blocking read until 5 chars received 

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
        outputnr++;
        if (outputnr > 2048)
        {
            outputnr = 0;
            time_t now;
            time(&now);
            printf("read %s %s" , portname, ctime(&now));
        }
#endif

        for (int i = 0; i < res; i++)
        {

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

            if (buf[i] == COLORADO_ERROR_CR)
            {
                printf("\n %02x b1 0d \n", COLORADO_ERROR_CR);
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
