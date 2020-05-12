#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"

//#define debug_header
//#define debug_lane
//#define debug_start
//#define debug_time

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#define DISPLAY_LANE_COUNT 8

#define MQTT_MESSAGE_LENGTH 8
// for the message to broker
#define MQTT_LONG_LENGTH 25

char COLORADO_HEADER_DATA[MQTT_LONG_LENGTH];
char COLORADO_HEAT_DATA[DISPLAY_LANE_COUNT][MQTT_MESSAGE_LENGTH];
char COLORADO_PLACE_DATA[DISPLAY_LANE_COUNT][2];

char *mydata;

int noworking;
bool running, stopping, pending;
uint8_t new_race_started;

int initanalyseData()
{
    printf("Load analyse data\n");
    //char *str;
    //str = (char *)malloc(sizeof(*str) * COLORADO_CHANNELS);

    int i;

    mydata = (char *)malloc(sizeof(char) * MQTT_LONG_LENGTH);
    for (i = 0; i < MQTT_LONG_LENGTH; i++)
    {
        mydata[i] = '0';
    }
    return 0;
}

int cleananalyseData()
{
    free(mydata);
    mqtt_clean();
    return 0;
}

bool analyseActiveData(uint8_t channel, uint8_t data[32])
{
    //MQTT_LONG_LENGTH
    char mydata[64];

#ifdef debug_lane
    sprintf(mydata, "channel: %d: 2: %d 4: %d 6: %d 8: %d 10: %d 12: %d 14: %d", channel, checkBitValue(data[2]), checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]), data[10], data[12], data[14]);
    printf("result: %s \n", mydata);
#endif

    sprintf(mydata, "%d: %d %d%d %d%d %d%d", channel, checkBitValue(data[2]), checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]), data[10], data[12], data[14]);
    getTime(channel, data);

    return true;
}

int getTime(int lane, uint8_t data[])
{
    //char mydata[MQTT_LONG_LENGTH];
    char shortdata[MQTT_MESSAGE_LENGTH] = "0000000";
    char log[MQTT_MESSAGE_LENGTH];
    char place[2];
    bool array_match = false;

    // Button ZEit
    //Problem ees wird die ZEit aus dem letzten lauf geschickt mit 0 als platz
    sprintf(shortdata, "%d%d%d%d%d%d", checkBitValue(data[4]), checkBitValue(data[14]), checkBitValue(data[12]), checkBitValue(data[10]), checkBitValue(data[8]), checkBitValue(data[6]));
    //sprintf(shortdata, "%d%d%d%d%d%d", checkBitValue(data[2]), checkBitValue(data[14]), checkBitValue(data[12]), checkBitValue(data[10]), checkBitValue(data[8]), checkBitValue(data[6]));
    sprintf(mydata, "lane %d %d%d:%d%d,%d%d %d", lane, checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]), checkBitValue(data[2]));
    sprintf(place, "%d", checkBitValue(data[2]));

#ifdef debug_lane
    printf("gettime: %s \n ", mydata);
#endif

    if (strcmp(shortdata, COLORADO_HEAT_DATA[lane - 1]) == 0)
    {
        array_match = true;

        if (strcmp(place, COLORADO_PLACE_DATA[lane - 1]) != 0)
        {
            //der platz ist ungleich
            if (strcmp(place, "0") != 0)
            {
                //jetzt müssen wir schicken
                array_match = false;
            }
        }
    }
    else
    {
        if (strcmp(shortdata, "000000") == 0)
        {
            array_match = true;
        }
        else
        {
            array_match = false;
        }
    }

    if (checkBitValue(data[2]) == 0 && checkBitValue(data[14]) == 13)
    {
        array_match = true;
    }

    if (!array_match)
    {
        if (mqtt_send(mydata))
        {
            printf("Error sending \n");
        }

        for (int i = 0; i < MQTT_MESSAGE_LENGTH; i++)
        {
            COLORADO_HEAT_DATA[lane - 1][i] = shortdata[i];
        }

        for (int i = 0; i < 2; i++)
        {
            COLORADO_PLACE_DATA[lane - 1][i] = place[i];
        }
    }
    return 0;
}

int8_t checkBitValue(int8_t data)
{
    if (data == 0x0F)
    {
        return 0x00;
    }
    return data;
};

void getHeader(uint8_t data[])
{
    char mydata[MQTT_LONG_LENGTH];
    bool array_match = true;

    sprintf(mydata, "header %d%d%d %d%d%d", checkBitValue(data[0]), checkBitValue(data[2]), checkBitValue(data[4]), checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]));

#ifdef debug_header
    printf("%02x %02x %02x %02x %02x %02x %02x %02x\n", data[0], data[2], data[4], data[6], data[8], data[10], data[12], data[14]);
    printf("Header: %s\n\n", mydata);
#endif

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

        //TODO clean Lane Data
        //nein es werden immer alle Zeiten bis zum erbrechen geschickt ...
        for (int clearnr = 0; clearnr < DISPLAY_LANE_COUNT; clearnr++)
        {
            sprintf(COLORADO_HEAT_DATA[clearnr], "000000");
            /* 
        COLORADO_HEAT_DATA[clearnr] = shortdata;
      for (int i = 0; i < MQTT_MESSAGE_LENGTH; i++)
      {
        COLORADO_HEAT_DATA[clearnr][i] = 0x00;
      }
      */
        }
    }

    if (!array_match)
    {
        mqtt_send(mydata);
        for (int i = 0; i < MQTT_LONG_LENGTH; i++)
        {
            COLORADO_HEADER_DATA[i] = mydata[i];
        }
    }
}

bool checknotnull(uint8_t data[])
{
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

bool checkStartStop(uint8_t data[])
{
    running = checknotnull(data);
    char mydata[MQTT_LONG_LENGTH];

    if (stopping != running)
    {
        if (running)
        {
#ifdef debug_start
            printf("start\n");
#endif
            sprintf(mydata, "start");
            mqtt_send(mydata);
            new_race_started = 0x01;
        }
        else
        {
#ifdef debug_start
            printf("stop\n");
#endif
            sprintf(mydata, "stop");
            mqtt_send(mydata);
        }
    }
    stopping = running;
    return true;
};

bool getTime(uint8_t data[])
{
    running = checknotnull(data);
    char mydata[MQTT_LONG_LENGTH];

    sprintf(mydata, "time %d%d:%d%d,%d", checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]));

    mqtt_send(mydata);
#ifdef debug_time
    printf("%s\n", mydata);
#endif
    return true;
};
