#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"

// #define debug

// 100/s
//  -> 10s
#define INACTIVE_TIME_AFTER_START 200
#define SEND_TIME_EVERY_COUNTS 20

int hundredth;
int loopcount;

bool sendActiveState;
bool headerChange;

void initRunninTime()
{
    // hundredth = 0;
    loopcount = 0;
    sendActiveState = false;
    headerChange = false;
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

    bool sendActiveState_save = sendActiveState;

    if (timehundredth > INACTIVE_TIME_AFTER_START)
    {
        sendActiveState = true;
    }

    if (timehundredth < INACTIVE_TIME_AFTER_START)
    {
        sendActiveState = false;
    }

    if (sendActiveState_save != sendActiveState && sendActiveState)
    {
        printf("Armed\n");
    }

    if (sendActiveState_save != sendActiveState && !sendActiveState)
    {
        printf("Paused\n");
    }

#ifdef debug
    printf("time 100 %d \n", timehundredth);
#endif

    return timehundredth;
}

void getTimeInternal(uint8_t data[])
{
    char mydata[64];
    // running = checknotnull(data);
    //  wir bauen bei laufwechsel das mit ein
    //  laufwechsel wird ausgeschaltet
    // hundredth = timehundredth(data);

    if (loopcount > SEND_TIME_EVERY_COUNTS)
    {

        if (!headerChange)
        {
            // setzen active state
            timehundredth(data);
        }
        // einmal warten zum reset
        headerChange = false;

        sprintf(mydata, "time %d%d:%d%d,%d", checkBitValue(data[4]), checkBitValue(data[6]),
                checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]));

        mqtt_send(mydata);
        loopcount = 0;
    }
    loopcount++;
};

void getTime(uint8_t *data[])
{
    getTimeInternal(*data);
};

bool getsendActiveState()
{
    return sendActiveState;
}

void setsendActiveStateOff()
{
    sendActiveState = false;
    headerChange = true;

#ifdef debug
    if (headerChange)
    {
        printf("header change - paused\n");
    }
#endif
}