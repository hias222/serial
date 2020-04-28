#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"

#ifdef _WIN32
#include <windows.h>
#include "SerialConfig.h"

#endif

static volatile int keepRunning;
static volatile int *ptrRunning;

void intHandler(int dummy)
{
    keepRunning = 0;
    ptrRunning = &keepRunning;
}

int main(int, char **)
{
    printf("serial %d.%d.%d \n", SERIAL_VERSION_MAJOR , SERIAL_VERSION_MINOR, SERIAL_VERSION_PATCH );
    keepRunning = 1;
    ptrRunning = &keepRunning;
    signal(SIGINT, intHandler);

    dataInit(ptrRunning);
    //dataStart();
    dataClean();
}
