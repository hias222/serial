#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"
#include "analyseLane.h"

//#define debug

int noworking;
bool pending, running;

bool b_running, b_stopping;

bool checknotnull(uint8_t data[])
{

    // nur jedes 5 mal
    noworking++;
    if (noworking > 5 || !pending)
    {

#ifdef debug
        printf("time %d \n", pending);
#endif
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

void checkStartStopInternal(uint8_t data[])
{
    b_running = checknotnull(data);
    char mydata[MQTT_LONG_LENGTH];

    if (b_stopping != b_running)
    {
        resetAllData();
        if (b_running)
        {
            sprintf(mydata, "start");
            mqtt_send(mydata);
        }
        else
        {
            sprintf(mydata, "stop");
            mqtt_send(mydata);
        }
    }
    b_stopping = b_running;
};

void checkStartStop(uint8_t *data[])
{
#ifdef debug
    printf("checkStartStop - start\n");
#endif
    checkStartStopInternal(*data);
#ifdef debug
    printf("checkStartStop - end\n");
#endif
}