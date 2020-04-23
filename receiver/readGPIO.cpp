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

#define COLORADO_ADDRESS_WORD_MASK 0x80

#define MODEMDEVICE "/dev/serial0"

using namespace std;

int read(char *portname, volatile int *running)
{

    int USBHandle, b, e;
    int order;
    char *TEXT;
    char text[2048];

    if (gpioInitialise() < 0)
    {
        fprintf(stderr, "pigpio initialisation failed.\n");
        return 1;
    }

    //bbSPIOpen(CE0, MISO, MOSI, SCLK, 10000, 0); // MCP4251 DAC
    //bbSPIOpen(CE1, MISO, MOSI, SCLK, 20000, 3); // MCP3008 ADC

    USBHandle = serOpen(MODEMDEVICE, 9600, 0);

    if (USBHandle < 0)
    {
        fprintf(stderr, "USBHandle failed.\n");
        return 1;
    }

    printf("   Serial port = %s\n", MODEMDEVICE);

    while (*running)
    {
        b = 0;
        while (b == 0)
        {
            b = serDataAvailable(USBHandle);
        }

        e = serRead(USBHandle, text, b); /* flush buffer */

        order = 0;

        if (e < 0)
        {
            printf("read failed.\n");
            return 1;
        }
        else
        {
            for (int i = 0; i < b; i++)
            {
                if ((text[i] & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
                {
                    printf("\n");
                    order = 0;
                }
                printf("%d: %02x ", order, text[i]);
                putReadData(text[i]);
                order++;
            }
            //printf("Text: %s", text);
        }
        //b = serReadByte(h);
        //b = serRead(USBHandle, text, strlen(TEXT));
    }

    serClose(USBHandle);

    //bbSPIClose(CE0);
    //bbSPIClose(CE1);

    gpioTerminate();

    return 0;
}
