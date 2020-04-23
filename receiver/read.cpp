#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define COLORADO_ADDRESS_WORD_MASK 0x80
#define BUFFER_LENGTH 32

#include <string.h>
#include <string>

// Linux headers
#include <errno.h> // Error integer and strerror() function

#include <unistd.h>

#include "analyseData.h"

#define BAUDRATE B38400
#define MODEMDEVICE "/dev/ttys002"
//#define MODEMDEVICE "/dev/ttyUSB1"
//#define MODEMDEVICE "/dev/cu.usbserial-14610"
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

volatile int STOP = FALSE;

using namespace std;

int read(char *portname, volatile int *running)
{
    int fd, c, res;
    struct termios oldtio, newtio;
    unsigned char buf[BUFFER_LENGTH];

    fd = open(MODEMDEVICE, O_RDONLY);
    if (fd < 0)
    {
        perror(MODEMDEVICE);
        return 1;
    }

    tcgetattr(fd, &oldtio); /* save current port settings */

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = ~CRTSCTS & ~PARENB & ~CSTOPB;
    newtio.c_cflag |= CS8 | CLOCAL | CREAD;
    cfsetispeed(&newtio, B4800);
    cfsetospeed(&newtio, B4800);

    //newtio.c_cflag = BAUDRATE | ~CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 0; /* inter-character timer unused */
    newtio.c_cc[VMIN] = 1;  /* blocking read until 5 chars received */

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    printf("\n");
    printf("   Serial port = %s\n", MODEMDEVICE);
    printf("\n");

    int g = 0;
    /*
    while (*running)
    {
        if (*running == 0)
        {
            printf("end");
        }
    }
    */

    while (*running)
    {
        res = read(fd, buf, BUFFER_LENGTH);        /* returns after 5 chars have been input */
        buf[res] = 0;

        for (int i = 0; i < res; i++)
        {
            if ((buf[i] & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
            {
                printf("\n");
                g = 0;
            }
            printf("%d: %02x ", g, buf[i]);
            putReadData(buf[i]);
            g++;
        }
        if (!*running){
            printf("\n\nexit\n");
        }
        fflush(stdout);

        //if (buf[0] == 'z')
        //  STOP = TRUE;
    }
    tcsetattr(fd, TCSANOW, &oldtio);
    return 1;
}