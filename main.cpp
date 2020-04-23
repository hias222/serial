#include <iostream>

#include "data.h"

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

