#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"
#include "analyseLane.h"

//#define debug

char *COLORADO_HEADER_DATA;

int mqttLongLength;

void initanalyseHeader( int mqttLength)
{
    int i;
    mqttLongLength = mqttLength;

    COLORADO_HEADER_DATA = (char *)malloc(sizeof(char) * mqttLongLength);
    for (i = 0; i < mqttLongLength; i++)
    {
        COLORADO_HEADER_DATA[i] = '0';
    }
}

void getHeaderInternal(uint8_t data[])
{
    char mydata[64];
    bool array_match = true;

    sprintf(mydata, "header %d%d%d %d%d%d", checkBitValue(data[0]), checkBitValue(data[2]), checkBitValue(data[4]), checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]));

    if (strcmp(mydata, "header 000 000") == 0)
    {
        sprintf(mydata, "clock");
    }
    else if (strcmp(mydata, COLORADO_HEADER_DATA) == 0)
    {
        array_match = true;
    }
    else
    {
        array_match = false;
        mqtt_send(mydata);
        printf("Header: %s\n", mydata);

        //TODO clean Lane Data
        //nein es werden immer alle Zeiten bis zum erbrechen geschickt ...
        resetAllData();
    }

    if (!array_match)
    {
        mqtt_send(mydata);
        for (int i = 0; i < mqttLongLength; i++)
        {
            COLORADO_HEADER_DATA[i] = mydata[i];
        }
    }
}

void getHeader(uint8_t *data[])
{
#ifdef debug
    printf("getHeader - start\n");
#endif
    getHeaderInternal(*data);
#ifdef debug
    printf("getHeader - end\n");
#endif
}