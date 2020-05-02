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
}

void usage(char *prog)
{
    printf("usage %s [-s %s] \n", prog, BASIC_PORTNAME);
    printf("  -s portname      source port name %s \n", BASIC_PORTNAME);
    printf("                   for both raw and repeater mode\n");
    printf("  -r               only behind RPI \n");
    printf("  -x               raw mode \n");
    printf("  -d portname      destination port name like %s \n", DESTINATION_PORTNAME);
    printf("                   only for raw mode\n");

}

int main(int argc, char *argv[])
{

    printf("serial %d.%d.%d \n", SERIAL_VERSION_MAJOR, SERIAL_VERSION_MINOR, SERIAL_VERSION_PATCH);
    char *portname = (char *)malloc(50);
    char *destinationportname = (char *)malloc(50);
    bool send_mode = false;
    bool repeat_mode = false;

    // out of SerialConfig
    sprintf(portname, "%s", BASIC_PORTNAME);
    sprintf(destinationportname, "%s", DESTINATION_PORTNAME);

    printf("init portnames\n");

    bool cmd_line_failure = true;

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
            case 'd':
                if (argc > n)
                {
                    destinationportname = argv[n + 1];
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

    if (SEND_MODE)
    {
        printf("turn on SENDMODE\n");
        send_mode = true;
    }

    keepRunning = 1;
    ptrRunning = &keepRunning;
    signal(SIGINT, intHandler);

    if (send_mode)
    {
        init_send(destinationportname);
        dataInit(ptrRunning, portname, true);
    }
    else if (repeat_mode)
    {
        printf("repeat\n");
        terminalInit(ptrRunning, portname);
    }
    else
    {
        dataInit(ptrRunning, portname, false);
    }

    dataClean();

    if (send_mode)
    {
        close_send();
    }

    free(portname);
    free(destinationportname);
}
