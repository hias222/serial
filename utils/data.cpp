#include <iostream>

#include "listener.h"
#include "analyseData.h"
#include "mqttUtils.h"
#include "data.h"
#include "terminalread.h"

static volatile int *keepRunning;

int dataInit(volatile int *running, char *portname, bool sendmode, bool verbose, bool ftdidevice)
{
    initReadData();
    mqtt_connect(sendmode);
    printf("utils - mqtt topic rawdata\n");
    char myText[] = "utils init repeat mode (mqtt rawdata)";
    mqtt_send(myText);
    startListen(running, portname, verbose, ftdidevice);
    return 0;
}

int terminalInit(volatile int *running, char *portname)
{
    mqtt_connect(false);
    //char myText[] = "header 1 1";
    //mqtt_send(myText);
    startTerminal(running, portname);
    return 0;
}

int dataClean()
{
    cleanReadData();
    return 0;
}

int dataStart()
{

    for (int f = 0; f < 10; f++)
    {
        //myData.setstart();
        for (int g = 0; g < 10; g++)
        {
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
            printf("lane\n");
            putReadData(0xb0);
            printf("lane data\n");
            putReadData(0x3);
            putReadData(0x1f);
            putReadData(0x2f);
            putReadData(0x49);
            putReadData(0x56);
            putReadData(0x4d);
            putReadData(0x6f);
            putReadData(0x7f);
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
        //next
        printf("header\n");
        putReadData(0x80);
    }
    return 0;
}
