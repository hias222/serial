#include <stdio.h>
#include <string.h>
#include <string>

#include "listener.h"
#include "setInterface.h"

#ifdef _WIN32
#include <process.h>
#include <Windows.h>
#include <iostream>
//#include "BaseWin.hh"
#else
#include <unistd.h>
//#include "Base.hh"
#endif

static volatile int *keepRunning;

bool testBasic()
{
    bool check=true;
    //bool check = checkPort();
    //TEST_ASSERT_THROW(check);
    //ALEPH_ASSERT_EQUAL(1, 2);
    return check;
}

void testAdvanced(char *portname)
{
    // a more advanced test
    //bool check = start(keepRunning, portname);
    //TEST_ASSERT_THROW(check);
}

int main(int, char **)
{
    char portname[] = "com1";
    if (testBasic())
    {
        testAdvanced(portname);
    }
}