#include <stdio.h>
#include <string.h>
#include <string>

#include "listener.h"

using namespace std;
#include "read.h"
#include "readftdi.h"

#ifdef _WIN32
#include <process.h>
#include <Windows.h>
#include <iostream>
#include <time.h>
#else
#include <unistd.h>
#endif

#include "info.h"

bool checkPort()
{
    bool check;
    check = false;

    printf("serialconnect - Start listener ..\n");
    printf("%s\n", getInfo().c_str());

    return check;
}

bool start(volatile int *running, char *portname, char *dstportname, bool forward, bool verbose, bool output)
{
    bool check = false;
    // char portname[] = "myport";
    try
    {
        read(portname, dstportname, forward, running, verbose, output);
        check = true;
    }
    catch (...)
    {
        printf("serialconnect - Read exception");
    }
    return check;
}

bool startftdi(volatile int *running, bool verbose, bool outputdata)
{
    bool check = false;
    try
    {
        readftdi(running, verbose, outputdata);
        check = true;
    }
    catch (...)
    {
        printf("serialconnect - Read exception");
    }
    return check;
}

int startListen(volatile int *running, char *portname, char *dstportname, bool verbose, bool ftdidevice, bool sendode, bool outputdata)
{
    printf("serialconnect - Serial start ...\n");
    // ToDo add send binary

    // ToDo Loop
    // for (int i = 0; i < 5; i++)
    while (*running)
    {
        time_t now;
        time(&now);
        printf("%s", ctime(&now));
        if (ftdidevice)
        {
            printf("serialconnect - ftdi running\n");
            startftdi(running, verbose, outputdata);
        }
        else
        {
            printf("serialconnect - serial running\n");
            start(running, portname, dstportname, sendode, verbose, outputdata);
        }

#ifdef _WIN32
        Sleep(5000);
        printf(".");
#else
        if (*running)
        {
            printf("\nserialconnect - checking\n");
            sleep(5);
        }
#endif
    }
    return 0;
}