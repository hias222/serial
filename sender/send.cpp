#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>

#define COLORADO_ADDRESS_WORD_MASK 0x80

#include <string.h>
#include <string>

// Linux headers
#include <errno.h> // Error integer and strerror() function

#include <unistd.h>
#include "setInterface.h"

//#define  MODEMDEVICE "/dev/ttyUSB0"
//#define MODEMDEVICE "/dev/ttys004"
//#define MODEMDEVICE "/dev/serial0"

int fd;
//struct termios oldtio;

#define BUFFER_LENGTH 25

int init_send(char *portname)
{

    //struct termios tty;
    printf("    port out = %s \n", portname);

    fd = open(portname, O_WRONLY);
    if (fd < 0)
    {
        perror(portname);
        return 1;
    }

    // bzero(&newtio, sizeof(newtio));

    if (set_interface_attribs(fd,B9600) > 0) {
        return 1;
    }

    tcflush(fd, TCIFLUSH);
    return 0;
}

int close_send()
{
    //tcsetattr(fd, TCSANOW, &oldtio);
    return 0;
}

int send(char *SendByte)
{
    int res;

    printf("   send: %s\n", SendByte);
    char endstring[] = ";";

    //unsigned 
    char text[] = "hello ;";

    //res = write(fd, SendByte, strlen(SendByte));
    res = write(fd, text, strlen(text));

    //res = write(fd, endstring, strlen(endstring));

    if (res < 0)
    {
        printf("error write\n");
        return 1;
    }

    printf ("   send %s\n", text);
    return 0;
}
