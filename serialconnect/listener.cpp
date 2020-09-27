#include <stdio.h>
#include <string.h>
#include <string>

#include "listener.h"

using namespace std;
#include "read.h"

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

    printf("Start listener ..\n");
    printf("%s\n", getInfo().c_str());

    return check;
}

bool start(volatile int *running, char *portname, bool verbose)
{
    bool check = false;
    //char portname[] = "myport";
    try
    {
        read(portname, running, verbose);
        check = true;
    }
    catch (...)
    {
        printf("Read exception");
    }
    return check;
}

int startListen(volatile int *running, char *portname, bool verbose)
{
    printf("Serial start ...\n");
    //ToDo Loop
    //for (int i = 0; i < 5; i++)
    while (*running)
    {
        time_t now;
        time(&now);
        printf("%s running ", ctime(&now));
        printf("\n");
        start(running, portname, verbose);
#ifdef _WIN32
        Sleep(5);
        printf(".");
#else
        if (*running)
        {
            printf("\nchecking\n");
            sleep(5);
        }
#endif
    }
    return 0;
}