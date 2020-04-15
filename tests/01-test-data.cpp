#include <stdio.h>

#ifdef _WIN32
#include <process.h>
#include <Windows.h>
#include <iostream>
#include "BaseWin.hh"
#else
#include <unistd.h>
#include "Base.hh"
#endif

#include "analyseData.h"
#include "mqttUtils.h"

int main()
{
    //char readData = { 0x80 };

    printf("Start testSerialData ..\n");

    mqtt_connect();

    //myData.setstart();
    printf("lane\n");
    putReadData(0xb0);
    printf("lane data\n");
    putReadData(0x3);
    putReadData(0x1f);
    putReadData(0x2f);
    putReadData(0x49);
    putReadData(0x56);
    putReadData(0x5d);
    putReadData(0x6f);
    putReadData(0x7f);
    // next
    printf("\time\n");
    putReadData(0xbe);
    printf("time data\n");
    putReadData(0xbe);
    putReadData(0xF);
    putReadData(0x1F);
    putReadData(0x2f);
    putReadData(0x39);
    putReadData(0x4c);
    putReadData(0x5d);
    //empty
    for (int i = 0; i < 120; i++)
    {
        putReadData(0xbe);
        putReadData(0xF);
        putReadData(0x1F);
        putReadData(0x2f);
        putReadData(0x3f);
        putReadData(0x4f);
        putReadData(0x5f);
    }
    //loop 0x0F
    for (int i = 0; i < 120; i++)
    {
        putReadData(0xbe);
        putReadData(0xF);
        putReadData(0x1F);
        putReadData(0x2f);
        putReadData(0x39);
        putReadData(0x4c);
        putReadData(0x5d);
    }
    //empty
    for (int i = 0; i < 120; i++)
    {
        putReadData(0xbe);
        putReadData(0xF);
        putReadData(0x1F);
        putReadData(0x2f);
        putReadData(0x3f);
        putReadData(0x4f);
        putReadData(0x5f);
    }

    //lane
    for (int i = 0; i < 10; i++)
    {
        putReadData(0xb0);
        printf("lane data\n");
        putReadData(0x3);
        putReadData(0x1f);
        putReadData(0x2f);
        putReadData(0x49);
        putReadData(0x56);
        putReadData(0x5d);
        putReadData(0x5f);
        putReadData(0x7f);
#ifdef _WIN32
        Sleep(1);
#else
        sleep(1);
#endif
        putReadData(0xb0);
        printf("lane data\n");
        putReadData(0x3);
        putReadData(0x1f);
        putReadData(0x2f);
        putReadData(0x49);
        putReadData(0x56);
        putReadData(0x5d);
        putReadData(0x6f);
        putReadData(0x7f);
    }

    //next
    printf("header\n");
    putReadData(0x80);
    /*
    printf("time data\n");
    //todo
    putReadData(0x38);
    putReadData(0x39);
    putReadData(0x38);
    putReadData(0x07);
    putReadData(0x07);
    putReadData(0x07);
    putReadData(0x07);
    putReadData(0x07);
    putReadData(0x07);
    putReadData(0x07);

    //more
    putReadData(0x80);
    */

    TEST_ASSERT_THROW(true);

    return 0;
}