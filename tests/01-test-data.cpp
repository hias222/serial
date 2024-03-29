#include <stdio.h>

#ifdef _WIN32
#include <process.h>
#include <Windows.h>
#include <iostream>
#include "BaseWin.hh"
#else
#include <unistd.h>
// #include "Base.hh"
#endif

#include "data.h"

static volatile int *keepRunning;

int main()
{
    char portname[] = "com1";
    char dstname[] = "com2";

    dataInit(keepRunning, portname, dstname, false, true, false, false);
    dataStart();
    dataClean();

    // TEST_ASSERT_THROW(true);

    return 0;
}