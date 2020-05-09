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

#include "setInterface.h"

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
    //struct termios tty;
    //unsigned 
    char buf[BUFFER_LENGTH];

    fd = open(portname, O_RDONLY);
    if (fd < 0)
    {
        perror(portname);
        return 1;
    }

    //tcgetattr(fd, &tty); /* save current port settings */
    set_interface_attribs(fd,B9600);

    tcflush(fd, TCIFLUSH);

    printf("\n");
    printf("    port in = %s\n", portname);
    printf("\n");

    int g = 0;

    char outgoing[BUFFER_LENGTH + 2];
    memset(&outgoing[0], 0, sizeof(outgoing));

    while (*running)
    {
        res = read(fd, buf, BUFFER_LENGTH); /* returns after 5 chars have been input */
        buf[res] = 0;

        if (res > 0)
        {
            printf("%s \n", buf);
            for (int i = 0; i < res; i++)
            {
                //printf("%02x \n", buf[i]);
                
                if (buf[i] == 0x3B || g > 24)
                {
                    g = 0;
                    printf("Terminal in: %s\n", outgoing);
                    mqtt_send(outgoing);
                    memset(&outgoing[0], 0, sizeof(outgoing));
                }
                else
                {
                    outgoing[g] = buf[i];
                    g++;
                }
            }
        }

        if (!*running)
        {
            printf("\n\nexit\n");
        }
        fflush(stdout);

        //if (buf[0] == 'z')
        //  STOP = TRUE;
    }

    free(outgoing);
    return 1;
}
