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

using namespace std;

bool read(string port)
{
    //init mem

    unsigned char loop = 0;
#ifdef __APPLE__
    // MAC dev/cu.usbserial-FTBTCOC2
    // socat /dev/ttys003
    //int serial_port = open("/dev/cu.usbserial-FTBTCOC2", O_RDWR);
    int serial_port = open("/dev/ttys003", O_RDWR);
#endif

#ifdef __linux__
    // RPI und Linux "/dev/ttyUSB0"
    int serial_port = open("/dev/ttyUSB0", O_RDWR);
#endif

    // Create new termios struc, we call it 'tty' for convention
    struct termios tty, newtio;
    memset(&tty, 0, sizeof tty);
    memset(&newtio, 0, sizeof newtio);

    // Read in existing settings, and handle any error
    if (tcgetattr(serial_port, &tty) != 0)
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return false;
    }

    /*
    8N1
    options.c_cflag &= ~PARENB
    options.c_cflag &= ~CSTOPB
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
*/
    tty.c_cflag &= ~PARENB;        // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB;        // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8;            // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS;       // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    /* Disable HArdware Flow
    options.c_cflag &= ~CNEW_RTSCTS;

    Raw Input
    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

    Input parity check
    options.c_iflag |= (INPCK | ISTRIP);

    Softwar FLOW CONTROL disable
    options.c_iflag &= ~(IXON | IXOFF | IXANY);

    https://tldp.org/HOWTO/Serial-Programming-HOWTO/x115.html
    Raw output.

    newtio.c_oflag = 0;
    ICANON  : enable canonical input
    disable all echo functionality, and don't send signals to calling program

    newtio.c_lflag = ICANON;
    */

    //tty.c_lflag &= ~ICANON;
    //tty.c_lflag &= ~ECHO;                                                        // Disable echo
    //tty.c_lflag &= ~ECHOE;                                                       // Disable erasure
    //tty.c_lflag &= ~ECHONL;                                                      // Disable new-line echo
    //tty.c_lflag &= ~ISIG;                                                        // Disable interpretation of INTR, QUIT and SUSP
    //tty.c_iflag &= ~(IXON | IXOFF | IXANY);                                      // Turn off s/w flow ctrl
    //tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL); // Disable any special handling of received bytes
    //tty.c_iflag &= IGNPAR | ICRNL;

    //tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    //tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    tty.c_oflag &= ~OPOST; //0
    tty.c_lflag &= ICANON;
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10; // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    //new ...
    bzero(&newtio, sizeof(newtio));
    newtio.c_cflag = BAUDRATE | CRTSCTS | CS8 | CLOCAL | CREAD;
    newtio.c_iflag = IGNPAR;
    newtio.c_oflag = 0;

    /* set input mode (non-canonical, no echo,...) */
    newtio.c_lflag = 0;

    newtio.c_cc[VTIME] = 100; /* inter-character timer unused 0 */
    newtio.c_cc[VMIN] = 0;  /* blocking read until 5 chars received 5 */

    tcflush(serial_port, TCIFLUSH);

    // Save tty settings, also checking for error
    //if (tcsetattr(serial_port, TCSANOW, &tty) != 0)
    if (tcsetattr(serial_port, TCSANOW, &newtio) != 0)
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return false;
    }

    // Allocate memory for read buffer, set size according to your needs
    //uint8_t read_buf[256];
    uint8_t ReadData;
    memset(&ReadData, '\0', sizeof(ReadData));

    // Read bytes. The behaviour of read() (e.g. does it block?,
    // how long does it block for?) depends on the configuration
    // settings above, specifically VMIN and VTIME
    int num_bytes;
    do
    {
        num_bytes = read(serial_port, &ReadData, sizeof(ReadData));
        printf("%02x (x)", ReadData);
        //putReadData(ReadData);
        ++loop;
    } while (num_bytes > 0);
    --loop; //Get Actual length of received data

    printf("\nNumber of bytes received = %d\n\n", loop);

    // int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0)
    {
        printf("Error reading: %s", strerror(errno));
        return false;
    }

    // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
    // print it to the screen like this!)
    //printf("Read %i bytes. Received message: %s", num_bytes, ReadData);

    close(serial_port);

    return true;
}
