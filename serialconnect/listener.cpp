#include <stdio.h>
#include <string.h>
#include <string>

#include "listener.h"
using namespace std;

#include "info.h"

#ifdef _WIN32
#include "serialwin.h"
#include "readwin.h"
#else
#include "read.h"
#include "serial.h"
#endif

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
    return check;
}

int startListen()
{
    //ToDo Loop
    start();
    return 0;
}