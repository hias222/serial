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

bool start(int volatile running)
{
    bool check = false;
    printf("Serial ...\n");
    char portname[] = "myport";
    try
    {
        read(portname, running);
        check = true;
        printf("Read success\n");
    }
    catch (...)
    {
        printf("Read exception");
    }
    printf("... Serial\n");
    return check;
}

int startListen(int volatile running)
{
    printf("Serial loop start!\n");
    //ToDo Loop
    for (int i = 0; i < 5; i++)
    {
        printf("Serial loop start! %d\n", i);
        start(running);
#ifdef _WIN32
        Sleep(5);
#else
        sleep(5);
#endif
    }

    return 0;
}