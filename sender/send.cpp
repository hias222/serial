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


//#define  MODEMDEVICE "/dev/ttyUSB0"
//#define MODEMDEVICE "/dev/ttys004"
//#define MODEMDEVICE "/dev/serial0"

int fd;
struct termios oldtio;

#define BUFFER_LENGTH 25

int init_send(char *portname)
{

    struct termios newtio;

    printf("    port out = %s \n", portname);

    fd = open(portname, O_RDWR | O_NOCTTY);
    if (fd < 0)
    {
        perror(portname);
        return 1;
    }

    tcgetattr(fd, &oldtio); /* save current port settings */

    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = ~CRTSCTS & ~PARENB & ~CSTOPB;
    newtio.c_cflag |= CS8 | CLOCAL | CREAD;
    cfsetispeed(&newtio, B9600);
    cfsetospeed(&newtio, B9600);

    //newtio.c_cflag = BAUDRATE | ~CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0 ; //|= ~OPOST;
    //newtio.c_oflag |= ~OPOST;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 0; /* inter-character timer unused */
    newtio.c_cc[VMIN] = 1;  /* blocking read until 5 chars received */

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);

    if (tcsetattr(fd, TCSANOW, &newtio) < 0)
    {
        fprintf(stderr, "\n");
        //explain_tcsetattr
        return 1;
    }

    return 0;
}

int close_send()
{
    tcsetattr(fd, TCSANOW, &oldtio);
    return 0;
}

int send(unsigned char *SendByte)
{
    int res;

    printf("   send: %s\n", SendByte);
    char endstring[2] = ";";

    //for (int i = 0; i < strlen(SendByte); i++){
    //    printf("%02x ", SendByte[i]);
    //}

    char mySendByte[] = "hello";

    //res = write(fd, mySendByte, size_t(SendByte));

    res = write(fd, mySendByte, strlen(mySendByte));

    res = write(fd, endstring, strlen(endstring));

    if (res < 0 ){
        printf("error write\n");
        return 1;
    }

    //unsigned char text = "hello";
    //char message[] = "\xbe\x70\x6f\x5f\x40\x30\x20\xa1\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xbc\x0e\x1e\x20\x30\x4d\x5d\x6a\x7e\xbd\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xba\x0d\x1a\x20\x30\x4c\x59\x66\x77\xbb\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb8\x0c\x19\x20\x30\x4b\x5f\x6e\x79\xb9\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb6\x0b\x18\x20\x30\x4b\x5c\x6d\x77\xb7\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xbe\x70\x6f\x5f\x40\x30\x20\xb4\x0a\x17\x20\x30\x4a\x5d\x6f\x78\xb5\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb2\x09\x1d\x20\x30\x4d\x58\x68\x7b\xb3\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xb0\x08\x1c\x20\x30\x4c\x5f\x6c\x79\xb1\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\xae\x07\x1b\x20\x30\x4c\x5c\x6b\x7d\xaf\x0d\x1f\x2f\x3f\x4d\x5d\x6f\x7f\x7f";
    //char buf[] = "\x80";

    //res = write(fd, buff, strlen(buf));
    //printf("write %d bytes\n", res);

    return 1;
}
