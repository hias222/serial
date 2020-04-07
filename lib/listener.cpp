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

bool startconnect()
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

    printf("Read ..\n");
    try
    {
        read(2);
    }
    catch (...)
    {
        printf("exception");
    }

    readtest(2);
    printf("End ..\n");
    return true;
}