#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"

//#define debug

bool b_running, b_stopping;

void checkStartStopInternal(uint8_t data[])
{
    b_running = checknotnull(data);
    char mydata[MQTT_LONG_LENGTH];

    if (b_stopping != b_running)
    {
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