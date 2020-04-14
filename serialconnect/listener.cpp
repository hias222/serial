#include <stdio.h>
#include <string.h>
#include <string>

#include "listener.h"

using namespace std;

#ifdef _WIN32
#include "serialwin.h"
#include "readwin.h"
#include <process.h>
#include <Windows.h>
#include <iostream>
#else
#include <unistd.h>
#include "read.h"
#include "serial.h"
#endif

#include "info.h"


bool checkPort()
{
    bool check;
    check = false;

    printf("Start listener ..\n");
    printf("%s\n", getInfo().c_str());

    check = open(1);

    if (check)
    {
        printf("success");
    }
    else
    {
        printf("open failure");
    }

    return check;
}

bool start()
{
    bool check = false;
    printf("Serial ...\n");
    try
    {
        read("2");
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

int startListen()
{
    printf("Serial loop start!\n");
    //ToDo Loop
    for (int i = 0; i< 5;i++){
        printf("Serial loop start! %d\n", i);
        start();
        sleep(5);
    }

    return 0;
}