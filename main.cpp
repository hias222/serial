#include <iostream>

#include "data.h"

static volatile int keepRunning;

void intHandler(int dummy) {
    keepRunning = 0;
}

int main(int, char **)
{
    keepRunning = 1;
    signal(SIGINT, intHandler);

    dataInit(keepRunning);
    //dataStart();
    dataClean();
}

