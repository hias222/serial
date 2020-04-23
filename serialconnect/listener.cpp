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

bool start(volatile int * running)
{
    bool check = false;
    char portname[] = "myport";
    try
    {
        read(portname, running);
        check = true;
    }
    catch (...)
    {
        printf("Read exception");
    }
    return check;
}

int startListen(volatile int *running)
{
    printf("Serial loop start!\n");
    //ToDo Loop
    //for (int i = 0; i < 5; i++)
    while(*running)
    {
        printf("\n");
        start(running);
#ifdef _WIN32
        Sleep(5);
#else
        sleep(5);
#endif
    }

    return 0;
}