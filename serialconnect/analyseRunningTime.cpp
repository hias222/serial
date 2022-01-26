#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"

// #define debug

int noworking;
bool pending;

bool running;
int hundredth;

void initRunninTime()
{

    hundredth = 0;
    running = false;
}

int timehundredth(uint8_t data[])
{

    int minutes;
    minutes = checkBitValue(data[4]) * 10 + checkBitValue(data[6]);

    int seconds;
    seconds = checkBitValue(data[8]) * 10 + checkBitValue(data[10]);

    int decent;
    decent = checkBitValue(data[12]);
    int timehundredth = (minutes * 60 + seconds) * 100 + decent * 10;

    return timehundredth;
}

bool checknotnull(uint8_t data[])
{
    // nur jedes 5 mal
    noworking++;
    if (noworking > 5)
    {
        noworking = 0;
        for (int i = 12; i > 4; i = i - 2)
        {
            if (data[i] != 0x0F)
            {
                if (data[i] > 0)
                {
                    pending = true;
                    return true;
                }
            }
        }
        pending = false;
        return false;
    }
    return pending;
};

void getTimeInternal(uint8_t data[])
{
    char mydata[64];
    running = checknotnull(data);
    hundredth = timehundredth(data);

    sprintf(mydata, "time %d%d:%d%d,%d", checkBitValue(data[4]), checkBitValue(data[6]),
            checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]));

    mqtt_send(mydata);
};

void getTime(uint8_t *data[])
{
#ifdef debug
    printf("getTime - start\n");
#endif
    getTimeInternal(*data);
#ifdef debug
    printf("getTime - end\n");
#endif
};

bool getRunningState()
{
    if (hundredth > 0)
    {
        return true;
    }
    return false;
}