#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define COLORADO_ADDRESS_WORD_MASK 0x80
#define BUFFER_LENGTH 25

#include <string.h>
#include <string>

//#define debug_incoming

// Linux headers
#include <errno.h> // Error integer and strerror() function
#include <unistd.h>

#include "terminalread.h"
#include "mqttUtils.h"

#define BAUDRATE B38400
//#define MODEMDEVICE "/dev/ttys002"
//#define MODEMDEVICE "/dev/ttyUSB0"
//#define MODEMDEVICE "/dev/cu.usbserial-14610"



using namespace std;

int startTerminal(volatile int *running, char *portname)
{
    printf("Terminal start ...\n");

    while (*running)
    {
        try
        {
            terminalread(portname, running);
        }
        catch (...)
        {
            printf("Read exception");
        }
#ifdef _WIN32
        Sleep(5);
#else
        sleep(5);
#endif
    }
    return 0;
}

int terminalread(char *portname, volatile int *running)
{
    int fd, c, res;
    struct termios oldtio, newtio;
    //unsigned 
    char buf[BUFFER_LENGTH];

    fd = open(portname, O_RDONLY);
    if (fd < 0)
    {
        perror(portname);
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
    newtio.c_lflag &= ~ICANON; // 0

    newtio.c_cc[VTIME] = 10; /* inter-character timer unused */
    newtio.c_cc[VMIN] = 0;   /* blocking read until 5 chars received */

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    printf("\n");
    printf("    port in = %s\n", portname);
    printf("\n");

    int g = 0;

    while (*running)
    {
        res = read(fd, buf, BUFFER_LENGTH); /* returns after 5 chars have been input */
        buf[res] = 0;

        printf("Terminal in: %s", buf);
        mqtt_send(buf);

        if (!*running)
        {
            printf("\n\nexit\n");
        }
        fflush(stdout);

        //if (buf[0] == 'z')
        //  STOP = TRUE;
    }
    tcsetattr(fd, TCSANOW, &oldtio);
    return 1;
}
