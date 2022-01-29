#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"

//#define debug

//100/s
#define INACTIVE_TIME_AFTER_START 1000
#define SEND_TIME_EVERY_COUNTS 30

int hundredth;
int loopcount;

bool sendActiveState;

void initRunninTime()
{
    hundredth = 0;
    loopcount = 0;
    sendActiveState = false;
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

    if (hundredth > INACTIVE_TIME_AFTER_START)
    {
        sendActiveState = true;
    }

    if (hundredth < INACTIVE_TIME_AFTER_START)
    {
        sendActiveState = false;
    }

#ifdef debug
    printf("time 100 %d \n", timehundredth);
#endif

    return timehundredth;
}

void getTimeInternal(uint8_t data[])
{
    char mydata[64];
    //running = checknotnull(data);
    hundredth = timehundredth(data);

    if (loopcount > SEND_TIME_EVERY_COUNTS)
    {
        sprintf(mydata, "time %d%d:%d%d,%d", checkBitValue(data[4]), checkBitValue(data[6]),
                checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]));

        mqtt_send(mydata);
        loopcount = 0;
    }
    loopcount++;
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

bool getsendActiveState()
{
    return sendActiveState;
}