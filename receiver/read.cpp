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
//#define info_read
//#define debug_incoming

volatile int STOP = FALSE;

using namespace std;

char *see_speed(speed_t speed)
{
    static char SPEED[20];
    switch (speed)
    {
    case B0:
        strcpy(SPEED, "B0");
        break;
    case B50:
        strcpy(SPEED, "B50");
        break;
    case B75:
        strcpy(SPEED, "B75");
        break;
    case B110:
        strcpy(SPEED, "B110");
        break;
    case B134:
        strcpy(SPEED, "B134");
        break;
    case B150:
        strcpy(SPEED, "B150");
        break;
    case B200:
        strcpy(SPEED, "B200");
        break;
    case B300:
        strcpy(SPEED, "B300");
        break;
    case B600:
        strcpy(SPEED, "B600");
        break;
    case B1200:
        strcpy(SPEED, "B1200");
        break;
    case B1800:
        strcpy(SPEED, "B1800");
        break;
    case B2400:
        strcpy(SPEED, "B2400");
        break;
    case B4800:
        strcpy(SPEED, "B4800");
        break;
    case B9600:
        strcpy(SPEED, "B9600");
        break;
    case B19200:
        strcpy(SPEED, "B19200");
        break;
    case B38400:
        strcpy(SPEED, "B38400");
        break;
    default:
        sprintf(SPEED, "unknown (%d)", (int)speed);
    }
    return SPEED;
}

void setAttributesOnSerial(int fd)
{
    struct termios sendtio;
    tcgetattr(fd, &sendtio);

    int baudrate;

    baudrate = B9600;
    cfsetospeed(&sendtio, baudrate);
    cfsetispeed(&sendtio, baudrate);

    //set into raw, no echo mode
    sendtio.c_iflag = 0;
    sendtio.c_lflag = 0;
    sendtio.c_oflag = 0;
    //sendtio.c_cflag = 0;
    // ftdi
    sendtio.c_cflag = PARENB;

    // for standard adapter
    //sendtio.c_cflag = ~PARENB;

    //INPCK  Enable input parity checking.
    //IGNPAR Ignore framing errors and parity errors.

    sendtio.c_cc[VTIME] = 1; // inter-character timer unused
    sendtio.c_cc[VMIN] = 0;  // blocking read until 5 chars received

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &sendtio);
}

void printAttributesOfSerialInterface(int fd)
{
    struct termios gettio;
    static char SPEED[20];
    speed_t speed;

    /*
    Colorado specific
    BaudRate = 9600 
    StopBits = 1 
    Parity   = none 
    */
    puts("BaudRate = 9600, StopBits = 1, Parity   = yes ??");

    if (tcgetattr(fd, &gettio) != 0)
        perror("tcgetatt() error");
    else
    {
        if (gettio.c_cflag & PARENB)
            puts("PARENB is used -> parity");
        else
            puts("PARENB is not used -> no parity");
        if (gettio.c_lflag & CSTOPB)
            puts("CSTOPB is set -> 2 stop bits");
        else
            puts("CSTOPB is not set -> 1 stop bit");
        printf("The end-of-file character is x'%02x'\n",
               gettio.c_cc[VEOF]);

        speed = cfgetispeed(&gettio);
        printf("BAUDRATE is %s\n",
               see_speed(speed));
    }
}

int read(char *portname, char *dstname, bool forward, volatile int *running, bool verbose)
{
    int fd, c, res, fo;
    int flag, baudrate;
    struct termios oldtio, newtio;

    unsigned char buf[BUFFER_LENGTH];

    printf("receiver - using serial read linux\n");

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
    setAttributesOnSerial(fd);

    if (forward)
    {
        fo = open(dstname, O_RDWR | O_NOCTTY | O_NDELAY);

        if (fo < 0)
        {
            perror(dstname);
            return 1;
        }

        setAttributesOnSerial(fo);
    }


    printf("\n");
    printf("    receiver - port in (USB)=%s\n", portname);
    printAttributesOfSerialInterface(fd);
    if (forward)
    {
        printf("    forward to - port in (USB)=%s\n", dstname);
        printAttributesOfSerialInterface(fo);
    }
    printf("\n");
    printf("receiver - check configured lanes on scoreboard \n");

    int g = 0;

    while (*running)
    {
        res = read(fd, buf, BUFFER_LENGTH); /* returns after 5 chars have been input */

        if (forward)
        {
            write(fo, buf, res);
        }

        buf[res] = 0;

#ifdef info_read
        outputnr++;
        if (outputnr > 2048)
        {
            outputnr = 0;
            time_t now;
            time(&now);
            printf("read %s %s", portname, ctime(&now));
        }
#endif

        for (int i = 0; i < res; i++)
        {

            if ((buf[i] & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
            {
                if (verbose)
                {
                    printf("\n");
                }
                g = 0;
            }

            if (verbose)
            {
                printf("%02x ", buf[i]);
            }

            putReadData(buf[i]);

            g++;
        }
        if (!*running)
        {
            printf("\n\nreceiver - exit\n");
        }
        fflush(stdout);
    }
    tcsetattr(fd, TCSANOW, &oldtio);
    return 1;
}
