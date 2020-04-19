#include <stdio.h>
#include <string.h>
#include <string>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function

#include <unistd.h>
#include <termios.h>
#include "analyseData.h"

#define BAUDRATE B9600
//#define MODEMDEVICE "/dev/ttys001"
#define MODEMDEVICE "/dev/ttyUSB0"

#define COLORADO_ADDRESS_WORD_MASK 0x80

using namespace std;

bool read(string port)
{
    //init mem

    char buf[255];

    unsigned char loop = 0;
#ifdef __APPLE__
    // MAC dev/cu.usbserial-FTBTCOC2
    // socat /dev/ttys003
    //int serial_port = open("/dev/cu.usbserial-FTBTCOC2", O_RDWR);
    int serial_port = open("/dev/cu.usbserial-14610", O_RDWR | O_NOCTTY);
    //int serial_port = open("/dev/ttys005", O_RDWR);
#endif

#ifdef __linux__
    // RPI und Linux "/dev/ttyUSB0"
    int serial_port = open(MODEMDEVICE, O_RDWR | O_NOCTTY);
#endif

    // Create new termios struc, we call it 'tty' for convention
    struct termios SerialPortSettings;

    if (tcgetattr(serial_port, &SerialPortSettings) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return false;
    }

    /* Setting the Baud rate */
    cfsetispeed(&SerialPortSettings, B9600); /* Set Read  Speed as 9600                       */
    cfsetospeed(&SerialPortSettings, B9600); /* Set Write Speed as 9600                       */

    /* 8N1 Mode */
    SerialPortSettings.c_cflag &= ~PARENB; /* Disables the Parity Enable bit(PARENB),So No Parity   */
    SerialPortSettings.c_cflag &= ~CSTOPB; /* CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit */
    SerialPortSettings.c_cflag &= ~CSIZE;  /* Clears the mask for setting the data size             */
    SerialPortSettings.c_cflag |= CS8;     /* Set the data bits = 8                                 */

    SerialPortSettings.c_cflag &= ~CRTSCTS;       /* No Hardware flow Control                         */
    SerialPortSettings.c_cflag |= CREAD | CLOCAL; /* Enable receiver,Ignore Modem Control lines       */

    SerialPortSettings.c_iflag = 0;
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY);         /* Disable XON/XOFF flow control both i/p and o/p */
    SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); /* Non Cannonical mode                            */
    SerialPortSettings.c_iflag &= IGNPAR;

    SerialPortSettings.c_oflag = 0;
    SerialPortSettings.c_oflag &= ~OPOST; // make raw

    SerialPortSettings.c_lflag = 0;

    /* Setting Time outs */
    SerialPortSettings.c_cc[VMIN] = 1;  /* Read at least 10 characters  wenn 0 - 45*/
    SerialPortSettings.c_cc[VTIME] = 0; /* Wait indefinetly   */

    tcflush(serial_port, TCIFLUSH);

    printf("ready with %s\n", MODEMDEVICE);
    fflush(stdout);

    if (tcsetattr(serial_port, TCSANOW, &SerialPortSettings) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return false;
    }
    else
    {
           printf("\n  BaudRate = 9600 \n  StopBits = 1 \n  Parity   = none \n");
    }

    uint8_t ReadData;
    memset(&ReadData, '\0', sizeof(ReadData));

    int num_bytes;
    do
    {
        num_bytes = read(serial_port, &ReadData, sizeof(ReadData));
        if ((ReadData & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
        {
            printf("\n");
        }
        putReadData(ReadData);
        ++loop;
    } while (num_bytes > 0);
    --loop; //Get Actual length of received data

    printf("\nNumber of bytes received = %d\n\n", loop);

    if (num_bytes < 0)
    {
        printf("Error reading: %s", strerror(errno));
        return false;
    }

    close(serial_port);

    return true;
}
