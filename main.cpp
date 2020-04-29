#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "SerialConfig.h"

#ifdef _WIN32
#include <windows.h>
#endif

static volatile int keepRunning;
static volatile int *ptrRunning;

void intHandler(int dummy)
{
    keepRunning = 0;
    ptrRunning = &keepRunning;
}

void usage(char *prog)
{
    printf("usage %s [-s %s] \n", prog, BASIC_PORTNAME);
}

int main(int argc, char *argv[])
{
    printf("serial %d.%d.%d \n", SERIAL_VERSION_MAJOR, SERIAL_VERSION_MINOR, SERIAL_VERSION_PATCH);
    char *portname = (char *)malloc(50);

    // out of SerialConfig
    sprintf(portname, "%s", BASIC_PORTNAME);

    bool cmd_line_failure = true;

    for (int n = 1; n < argc; n++) /* Scan through args. */
    {
        switch ((int)argv[n][0]) /* Check for option character. */
        {
        case '-':
            switch ((int)argv[n][1]) /* Check for option character. */
            {
            case 's':
                printf("we getting source \n");
                if (argc > n)
                {
                    portname = argv[n + 1];
                    cmd_line_failure = false;
                }
                break;
            default:
                usage(argv[0]);
                return 0;
                break;
            }
            break;
        default:
            if (cmd_line_failure)
            {
                usage(argv[0]); /* Not option -- text. */
                return 0;
            }
            break;
        }
    }

    keepRunning = 1;
    ptrRunning = &keepRunning;
    signal(SIGINT, intHandler);

    dataInit(ptrRunning, portname);
    //dataStart();
    dataClean();

    free(portname);
}
