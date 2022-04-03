#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"
#include "analyseRunningTime.h"

#define debug_info
// ###############################
// für Reihenfolge nach Platz
// #define orderbyplace
// ################################
//#define debug_send
//#define debug
//#define debug_trace
//#define debug_trace

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

char **coloradoHeatData;
char **coloradoPlaceData;
uint8_t *lanebyplaceorder;

int DISPLAY_LANE_COUNT;
int MQTT_MESSAGE_LENGTH;

void resetTimeAndPlace(uint8_t lane)
{
    if (lane == 0)
    {
        printf("internal error lane 0 resetTimeAndPlace\n");
    }
    else
    {
        char setshortdata[8];
        sprintf(setshortdata, "%d%d%d%d%d%d", getNullBit(), getNullBit(), getNullBit(), getNullBit(), getNullBit(), getNullBit());
        char nullplace[3];
        sprintf(nullplace, "%d", getNullBit());

        // coloradoHeatData[lane - 1] = nullshortdata;
        strncpy(coloradoPlaceData[lane - 1], nullplace, 3);
        strncpy(coloradoHeatData[lane - 1], setshortdata, 8);
    }
}

void resetAllData()
{
    // first prevent data send
    for (int clearnr = 0; clearnr < DISPLAY_LANE_COUNT; clearnr++)
    {
        resetTimeAndPlace(clearnr + 1);
        // sprintf(coloradoHeatData[clearnr], "000000");
    }
}

void initanalyseLane(int laneCount, int mqqMsgLength)
{
    int i;
    DISPLAY_LANE_COUNT = laneCount;
    MQTT_MESSAGE_LENGTH = mqqMsgLength;

    printf("analyseLane with %d lanes and %d message size \n", laneCount, mqqMsgLength);

    //    char shortdata[MQTT_MESSAGE_LENGTH] = "0000000";

    lanebyplaceorder = (uint8_t *)malloc(sizeof(uint8_t) * 12);

    coloradoHeatData = (char **)malloc(sizeof(char *) * DISPLAY_LANE_COUNT);
    for (i = 0; i < DISPLAY_LANE_COUNT; i++)
    {
        // in case for special characters +8
        coloradoHeatData[i] = (char *)malloc(sizeof(char) * (18));
        char setshortdata[12];
        sprintf(setshortdata, "%d%d%d%d%d%d", getNullBit(), getNullBit(), getNullBit(), getNullBit(), getNullBit(), getNullBit());
        strncpy(coloradoHeatData[i], setshortdata, 12);
    }

    coloradoPlaceData = (char **)malloc(sizeof(char *) * DISPLAY_LANE_COUNT);
    for (i = 0; i < DISPLAY_LANE_COUNT; i++)
    {
        // in case for special characters +2
        coloradoPlaceData[i] = (char *)malloc(sizeof(char) * (10));
        char nullplace[3];
        sprintf(nullplace, "%d", getNullBit());
        strncpy(coloradoPlaceData[i], nullplace, 3);
    }

    // resetAllData();
}

void cleananalyseLane()
{
    free(coloradoPlaceData);
    free(coloradoHeatData);
    free(lanebyplaceorder);
}

void sendLaneData(char mydata[])
{

#ifdef debug_trace
    printf("getLaneTime - step 3a\n");
#endif
    // wir schicken nur wenn die uhr läuft
    // if (hundredth > 500)
    if (getsendActiveState())
    {

#ifdef debug_trace
        printf("getLaneTime - step 3b\n");
#endif
        if (mqtt_send(mydata))
        {
#ifdef debug_trace
            printf("getLaneTime - step 3c\n");
#endif
            printf("Error sending \n");
        }
    }
#ifdef debug_info
    else
    {
        printf("no running state - ");
    }
#endif
#ifdef debug_trace
    printf("getLaneTime - step 3d\n");
#endif

#ifdef debug_info
    printf("sendLaneData - %s \n", mydata);
#endif
}

void getLaneTime(uint8_t lane, uint8_t data[])
{
    char nullshortdata[7];
    sprintf(nullshortdata, "%d%d%d%d%d%d", 0, 0, 0, 0, 0, 0);
    char nullplace[3];
    sprintf(nullplace, "%d", 0);
    char place[3];

    bool array_match = false;
    char mydata[64];
    char mydata_2[64];
    char shortdata[16];

    char ordereddata[64];

    // Button Zeit
    // Problem ees wird die Zeit aus dem letzten lauf geschickt mit 0 als platz
    sprintf(shortdata, "%d%d%d%d%d%d", checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]),
            checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]));

#ifdef orderbyplace
    // data in place order
    // if 0x0F -> cleared
    if (data[2] != 0x0F)
    {
        sprintf(mydata, "lane %d %d%d:%d%d,%d%d %d", checkBitValue(data[2]), checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]),
                checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]), lane);
        sprintf(place, "%d", lane);
        lanebyplaceorder[lane] = checkBitValue(data[2]);
        // printf("--- %d (row: %d - %d)\n", lanebyplaceorder[lane], lane, data[2]);
    }
    else
    {
        sprintf(mydata, "lane %d %d%d:%d%d,%d%d %d", lanebyplaceorder[lane], checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]),
                checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]), checkBitValue(data[2]));
        sprintf(place, "%d", 0);
    }

#else
    // data in lane order
    sprintf(mydata, "lane %d %d%d:%d%d,%d%d %d", lane, checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]),
            checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]), checkBitValue(data[2]));
    sprintf(place, "%d", checkBitValue(data[2]));
#endif

    if (strcmp(shortdata, coloradoHeatData[lane - 1]) == 0)
    {
        array_match = true;
        if (strcmp(place, coloradoPlaceData[lane - 1]) != 0)
        {
            // der platz ist ungleich 0
            if (strcmp(place, nullplace) != 0)
            {
                // jetzt müssen wir schicken
                array_match = false;
            }
        }
    }
    else
    {
        if (strcmp(shortdata, nullshortdata) == 0)
        {
            array_match = true;
            resetTimeAndPlace(lane);
        }
        else
        {
#ifdef debug_send
            printf("not matching and time not null - %s\n", shortdata);
            printf("stored time                    - %s\n", coloradoHeatData[lane - 1]);
#endif
            array_match = false;
        }

#ifdef orderbyplace
        if (strcmp(place, nullplace) == 0)
        {
            // immer schicken bei placeorder
            array_match = false;
            printf("cleared %d\n", lanebyplaceorder[lane]);
        }
#endif
    }
    // ???
    if (checkBitValue(data[2]) == 0 && checkBitValue(data[14]) == 13)
    {
        array_match = true;
    }

    if (!array_match)
    {
#ifdef debug_send
        printf("prepare data - %s -- %s -- %s\n", shortdata, coloradoHeatData[lane - 1], coloradoPlaceData[lane - 1]);
#endif
        sendLaneData(mydata);
        array_match = true;
        strncpy(coloradoHeatData[lane - 1], shortdata, 8);
        // coloradoHeatData[lane - 1] = shortdata;
        strncpy(coloradoPlaceData[lane - 1], place, 3);
        // coloradoPlaceData[lane - 1] = place;

#ifdef orderbyplace
        // Wenn 2 Platz kommt ersten platz reseten
        if (lane == 2 && data[2] == 0x0F)
        {
            sprintf(mydata_2, "lane %d %d%d:%d%d,%d%d %d", lanebyplaceorder[1], checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]),
                    checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]), checkBitValue(data[2]));
            sendLaneData(mydata_2);
        }
#endif

#ifdef debug_send
        printf("send data - %s -- %s -- %s\n", shortdata, coloradoHeatData[lane - 1], coloradoPlaceData[lane - 1]);
#endif
    }
}

void analyseActiveData(uint8_t channel, uint8_t *data[])
{
#ifdef debug
    printf("analyseLane - analyseActiveData - start\n");
    showDisplayInternal(*data);
#endif

    getLaneTime(channel, *data);

#ifdef debug
    printf("analyseLane - analyseActiveData - end\n");
#endif
}