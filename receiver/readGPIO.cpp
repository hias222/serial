#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#include "pigpio.h"
#include "analyseData.h"

#define CE0 5
#define CE1 6
#define MISO 13
#define MOSI 19
#define SCLK 12
// #define debug_incoming
//#define info_read

#define COLORADO_ADDRESS_WORD_MASK 0x80

//#define MODEMDEVICE "/dev/serial0"

using namespace std;

int read(char *portname, volatile int *running, bool verbose)
{

    int USBHandle, b, e;
    int order;
    char *TEXT;
    char text[2048];

    printf("receiver - using gpio RPI \n");
    if (verbose)
    {
        printf("receiver - verbose mode  \n");
    }

#ifdef info_read
    int outputnr;
    outputnr = 9999;
#endif

    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed.\n");
        return 1;
    }

    USBHandle = serOpen(portname, 9600, 0);

    if (USBHandle < 0)
    {
        fprintf(stderr, "USBHandle failed.\n");
        return 1;
    }

    printf("   receiver - Serial port (GPIO) = %s\n", portname);

    while (*running)
    {
        b = 0;
        while (b == 0)
        {
            b = serDataAvailable(USBHandle);
        }

        e = serRead(USBHandle, text, b); /* flush buffer */

        order = 0;

#ifdef info_read
        outputnr++;
#endif

        if (e < 0)
        {
            printf("read failed.\n");
            return 1;
        }
        else
        {

#ifdef info_read
            if (outputnr > 512)
            {
                outputnr = 0;
                time_t now;
                time(&now);
                printf("%s getting 512 bytes", ctime(&now));
                printf("\n");
            }
#endif
            for (int i = 0; i < b; i++)
            {

                if ((text[i] & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
                {
                    if (verbose)
                    {
                        printf("\n");
                    }
#ifdef debug_incoming
                    printf("\n %02x \n", COLORADO_ADDRESS_WORD_MASK);
#endif
                    order = 0;
                }

                if (verbose)
                {
                    printf("%02x ", text[i]);
                }
                putReadData(text[i]);

                order++;
            }
        }
    }

    serClose(USBHandle);
    gpioTerminate();

    return 0;
}
