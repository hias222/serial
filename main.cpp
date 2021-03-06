#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"
#include "SerialConfig.h"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef SEND_MODE
#include "send.h"
#endif

static volatile int keepRunning;
static volatile int *ptrRunning;

void intHandler(int dummy)
{
    keepRunning = 0;
    ptrRunning = &keepRunning;

    printf("received exit\n");
}

void usage(char *prog)
{
    printf("usage %s [-s %s] \n", prog, BASIC_PORTNAME);
    printf("  -s portname      source port name %s \n", BASIC_PORTNAME);
    printf("                   for both raw and repeater mode\n");
    printf("                   RPI Ubuntu /dev/ttyAMA0 RPI raspian /dev/serial0  Linux /dev/ttyUSB0\n");
    printf("  -f               using ftdi library no port needed \n");
    printf("  -r               only behind RPI \n");
    printf("  -x               raw mode and send (default) \n");
    printf("  -z               only raw mode \n");
    printf("  -v               verbose output for serial \n");
    printf("  -d portname      destination port name like %s \n", DESTINATION_PORTNAME);
    printf("                   only for raw mode\n");
    printf("  -h               help \n");
    printf("\n");
    printf("data is published to localhost:1883 on queue rawdata\n");
}

int main(int argc, char *argv[])
{

    printf("serial %d.%d.%d \n", SERIAL_VERSION_MAJOR, SERIAL_VERSION_MINOR, SERIAL_VERSION_PATCH);
    char *portname = (char *)malloc(50);
    char *destinationportname = (char *)malloc(50);
    bool send_mode = false;
    bool repeat_mode = false;
    bool ftdi_mode = false;

    // out of SerialConfig
    sprintf(portname, "%s", BASIC_PORTNAME);
    sprintf(destinationportname, "%s", DESTINATION_PORTNAME);

    bool cmd_line_failure = true;
    bool cmd_verbose_mode = false;

    if (SEND_MODE)
    {
        printf("main - compiled with SENDMODE override with z\n");
        send_mode = true;
    }

    for (int n = 1; n < argc; n++) /* Scan through args. */
    {
        switch ((int)argv[n][0]) /* Check for option character. */
        {
        case '-':
            switch ((int)argv[n][1]) /* Check for option character. */
            {
            case 's':
                //printf("we getting source \n");
                if (argc > n)
                {
                    portname = argv[n + 1];
                    cmd_line_failure = false;
                }
                break;
            case 'f':
                //printf("we getting source \n");
                if (argc > n)
                {
                    ftdi_mode = true;
                }
                break;
            case 'r':
                if (argc > n)
                {
                    repeat_mode = true;
                    send_mode = false;
                    cmd_line_failure = false;
                }
                break;
            case 'x':
                if (argc > n)
                {
                    repeat_mode = false;
                    send_mode = true;
                    cmd_line_failure = false;
                }
                break;
            case 'z':
                if (argc > n)
                {
                    repeat_mode = false;
                    send_mode = false;
                    cmd_line_failure = false;
                }
                break;
            case 'd':
                if (argc > n)
                {
                    destinationportname = argv[n + 1];
                    cmd_line_failure = false;
                }
                break;
            case 'v':
                cmd_verbose_mode = true;
                break;
            case 'h':
                usage(argv[0]);
                return 0;
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

    if (ftdi_mode)
    {
        printf("main - receive data on ftdi device\n");
    }
    else
    {
        printf("main - receive colorado data on port %s\n", portname);
    }

    keepRunning = 1;
    ptrRunning = &keepRunning;
    signal(SIGINT, intHandler);

    if (send_mode)
    {
        printf("main - send - send out to port %s\n", destinationportname);
        init_send(destinationportname);
        dataInit(ptrRunning, portname, true, cmd_verbose_mode, ftdi_mode);
    }
    else if (repeat_mode)
    {
        printf("main - repeat - receive on port %s\n", portname);
        terminalInit(ptrRunning, portname);
    }
    else
    {
        printf("main - raw local apply to mqttt localhost:1883 topic rawdata\n");
        dataInit(ptrRunning, portname, false, cmd_verbose_mode, ftdi_mode);
    }

    dataClean();

    if (send_mode)
    {
        close_send();
    }

    free(portname);
    free(destinationportname);
}
