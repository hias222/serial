#include <iostream>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include "data.h"

#ifdef _WIN32
#include <windows.h>

#endif

static volatile int keepRunning;
static volatile int* ptrRunning;

void intHandler(int dummy) {
    keepRunning = 0;
    ptrRunning = &keepRunning;
}

int main(int, char **)
{
    keepRunning = 1;
    ptrRunning = &keepRunning;
    signal(SIGINT, intHandler);

    dataInit(ptrRunning);
    //dataStart();
    dataClean();
}

