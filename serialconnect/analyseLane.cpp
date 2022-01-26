#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"
#include "analyseRunningTime.h"

#define debug_send
//#define debug
//#define debug_trace
//#define debug_trace

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

char **COLORADO_HEAT_DATA;
char **COLORADO_PLACE_DATA;

int DISPLAY_LANE_COUNT;
int MQTT_MESSAGE_LENGTH;

#define TIME_STORE_LENGTH 8
#define PLACE_LENGTH 3

void resetTimeAndPlace(uint8_t lane)
{
    char nullshortdata[TIME_STORE_LENGTH];
    sprintf(nullshortdata, "%d%d%d%d%d%d", 0, 0, 0, 0, 0, 0);
    COLORADO_HEAT_DATA[lane - 1] = nullshortdata;

    //char nullplace[3];
    //sprintf(nullplace, "%d", 0);
    //COLORADO_PLACE_DATA[lane - 1] = nullplace;

}

void resetAllData()
{
    for (int clearnr = 0; clearnr < DISPLAY_LANE_COUNT; clearnr++)
    {
        resetTimeAndPlace(clearnr);
        //sprintf(COLORADO_HEAT_DATA[clearnr], "000000");
    }
}

void initanalyseLane(int laneCount, int mqqMsgLength)
{
    int i;
    DISPLAY_LANE_COUNT = laneCount;
    MQTT_MESSAGE_LENGTH = mqqMsgLength;

    printf("analyseLane with %d lanes and %d message size ", laneCount, mqqMsgLength);

    //    char shortdata[MQTT_MESSAGE_LENGTH] = "0000000";

    COLORADO_HEAT_DATA = (char **)malloc(sizeof(char *) * DISPLAY_LANE_COUNT);
    for (i = 0; i < DISPLAY_LANE_COUNT; i++)
    {
        // in case for special characters +8
        COLORADO_HEAT_DATA[i] = (char *)malloc(sizeof(char) * (TIME_STORE_LENGTH));
    }

    COLORADO_PLACE_DATA = (char **)malloc(sizeof(char *) * DISPLAY_LANE_COUNT);
    for (i = 0; i < DISPLAY_LANE_COUNT; i++)
    {
        // in case for special characters +2
        COLORADO_PLACE_DATA[i] = (char *)malloc(sizeof(char) * (PLACE_LENGTH + 2));
    }

    resetAllData();
}

void cleananalyseLane()
{
    free(COLORADO_PLACE_DATA);
    free(COLORADO_HEAT_DATA);
}

void sendLaneData(char mydata[])
{

#ifdef debug_trace
    printf("getLaneTime - step 3a\n");
#endif
    // wir schicken nur wenn die uhr läuft
    //if (hundredth > 0)
    if (getRunningState())
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
#ifdef debug_send
    else
    {
        printf("no running state - ");
    }
#endif
#ifdef debug_trace
    printf("getLaneTime - step 3d\n");
#endif

#ifdef debug_send
    printf("sendLaneData - %s \n", mydata);
#endif
}

void getLaneTime(uint8_t lane, uint8_t data[])
{
    char nullshortdata[TIME_STORE_LENGTH];
    sprintf(nullshortdata, "%d%d%d%d%d%d", 0, 0, 0, 0, 0, 0);
    char nullplace[3];
    sprintf(nullplace, "%d", 0);

    char log[MQTT_MESSAGE_LENGTH];
    char place[PLACE_LENGTH];
    bool array_match = false;
    char mydata[64];
    char shortdata[TIME_STORE_LENGTH];

    // Button ZEit
    //Problem ees wird die Zeit aus dem letzten lauf geschickt mit 0 als platz
    sprintf(shortdata, "%d%d%d%d%d%d", checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]),
            checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]));
    sprintf(mydata, "lane %d %d%d:%d%d,%d%d %d", lane, checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]),
            checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]), checkBitValue(data[2]));
    sprintf(place, "%d", checkBitValue(data[2]));

    if (strcmp(shortdata, COLORADO_HEAT_DATA[lane - 1]) == 0)
    {
        array_match = true;
        if (strcmp(place, COLORADO_PLACE_DATA[lane - 1]) != 0)
        {
            //der platz ist ungleich 0
            if (strcmp(place, nullplace) != 0)
            {
                //jetzt müssen wir schicken
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
            printf("not matching and time not null (000000) - %s -- %s\n", shortdata, COLORADO_HEAT_DATA[lane - 1]);
#endif
            array_match = false;
        }
    }
    // ???
    if (checkBitValue(data[2]) == 0 && checkBitValue(data[14]) == 13)
    {
        array_match = true;
    }

    if (!array_match)
    {
        sendLaneData(mydata);
        array_match = true;
        COLORADO_HEAT_DATA[lane - 1] = shortdata;
        //for (int i = 0; i < 3; i++)
        //{
        COLORADO_PLACE_DATA[lane - 1] = place;
        //}
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