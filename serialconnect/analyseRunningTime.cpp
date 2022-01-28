#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"

//#define debug

int hundredth;

bool sendActiveState;

void initRunninTime()
{
    hundredth = 0;
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

    if (hundredth > 500)
    {
        sendActiveState = true;
    }

    if (hundredth < 500)
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

bool getsendActiveState()
{
    return sendActiveState;
}