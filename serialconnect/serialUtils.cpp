#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"

//#define debug_header
//#define debug_lane_place
//#define debug_lane
//#define debug_start
//#define debug_time

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#define DISPLAY_LANE_COUNT 10
#define BUFFER_LENGTH 16

#define MQTT_MESSAGE_LENGTH 8
// for the message to broker
#define MQTT_LONG_LENGTH 25

char COLORADO_HEADER_DATA[MQTT_LONG_LENGTH];
char COLORADO_HEAT_DATA[DISPLAY_LANE_COUNT][MQTT_MESSAGE_LENGTH];
char COLORADO_PLACE_DATA[DISPLAY_LANE_COUNT][3];

char *mydata;
char *storeRoundsData;
int round;

int noworking;
bool running, stopping, pending;
int hundredth;

int initanalyseData()
{
    printf("serialconnect - Load analyse data\n");
    //char *str;
    //str = (char *)malloc(sizeof(*str) * COLORADO_CHANNELS);

    int i;
    hundredth = 0;
    running = false;

    //round = malloc(cnt * sizeof *round);
    round = 0;

    mydata = (char *)malloc(sizeof(char) * MQTT_LONG_LENGTH);
    storeRoundsData = (char *)malloc(sizeof(char) * MQTT_LONG_LENGTH);
    for (i = 0; i < MQTT_LONG_LENGTH; i++)
    {
        mydata[i] = '0';
        storeRoundsData[i] = '0';
    }

    return 0;
}

int cleananalyseData()
{
    //free(mydata);
    //free(storeRoundsData);
    mqtt_clean();
    return 0;
}

bool analyseActiveData(uint8_t channel, uint8_t *data[32])
{
    //MQTT_LONG_LENGTH
    char mydata[64];

#ifdef debug_lane
    sprintf(mydata, "channel: %d: 2: %d 4: %d 6: %d 8: %d 10: %d 12: %d 14: %d", channel, checkBitValue(data[2]), checkBitValue(data[4]),
            checkBitValue(data[6]), checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]));
    printf("result: %s \n", mydata);

    printf("              ");
    for (int i = 0; i < BUFFER_LENGTH; i++)
    {
        printf("%02x ", data[i]);
    }
    printf("\n");

#endif

#ifdef debug_lane_place

    if (channel == 0x01)
    {
        printf("channel %d \n", channel);
    }

#endif

    //showDisplayLine(*data);

    getTime(channel, *data);

    return true;
}

int getTime(uint8_t lane, uint8_t data[])
{
    //char mydata[MQTT_LONG_LENGTH];
    char shortdata[MQTT_MESSAGE_LENGTH] = "0000000";
    char log[MQTT_MESSAGE_LENGTH];
    char place[3];
    bool array_match = false;
    char nullplace[3] = "0";

    // Button ZEit
    //Problem ees wird die ZEit aus dem letzten lauf geschickt mit 0 als platz
    sprintf(shortdata, "%d%d%d%d%d%d", checkBitValue(data[4]), checkBitValue(data[14]), checkBitValue(data[12]), checkBitValue(data[10]), checkBitValue(data[8]), checkBitValue(data[6]));
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
            //der platz ist ungleich 0

            if (strcmp(place, nullplace) != 0)
            {
#ifdef debug_lane_place
                printf("change place not null %d place %s - %s - %s\n", lane, place, nullplace, COLORADO_PLACE_DATA[lane - 1]);
#endif
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
            restTimeAndplace(lane);
        }
        else
        {
#ifdef debug_lane_place
            debugLane(lane, shortdata, data);
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
        array_match = true;
        // wir schicken nur wenn die uhr läuft
        if (hundredth > 0)
        {
            if (mqtt_send(mydata))
            {
                printf("Error sending \n");
            }
        }
        else
        {
#ifdef debug_time
            printf("no data transfer clock : %d\n", hundredth);
#endif
        }

        for (int i = 0; i < MQTT_MESSAGE_LENGTH; i++)
        {
            COLORADO_HEAT_DATA[lane - 1][i] = shortdata[i];
        }

        for (int i = 0; i < 3; i++)
        {
            COLORADO_PLACE_DATA[lane - 1][i] = place[i];
        }
    }

    return 0;
}

uint8_t checkBitValue(uint8_t data)
{
    if (data == 0x0F)
    {
        return 0x00;
    }
    return data;
};

void restTimeAndplace(uint8_t lane)
{
    char shortdata[MQTT_MESSAGE_LENGTH] = "0000000";
    char place[2] = "0";

    //reset
    for (int i = 0; i < DISPLAY_LANE_COUNT; i++)
    {
        COLORADO_PLACE_DATA[lane - 1][i] = place[i];
        COLORADO_HEAT_DATA[lane - 1][i] = shortdata[i];
    }
}

void storeRounds(uint8_t data[])
{
    char mydata[MQTT_LONG_LENGTH];
    char sendData[MQTT_LONG_LENGTH];
    bool array_match = true;

    sprintf(mydata, "%d%d%d%d%d%d%d%d", checkBitValue(data[0]), checkBitValue(data[2]), checkBitValue(data[4]),
            checkBitValue(data[6]), checkBitValue(data[8]), checkBitValue(data[10]),
            checkBitValue(data[12]), checkBitValue(data[14]));
    //printf("showDisplayLine\n");
    //printf("--> %s\n", mydata);

    if (strcmp(mydata, "00000000") == 0)
    {
        round = 0;
        if (strcmp(mydata, storeRoundsData) != 0)
        {
            strcpy(storeRoundsData, mydata);
            sprintf(sendData, "round %d", round);
            mqtt_send(sendData);
        }
    }

    if (strcmp(mydata, storeRoundsData) != 0)
    {
        round++;
        strcpy(storeRoundsData, mydata);
        sprintf(sendData, "round %d", round);
        mqtt_send(sendData);
    }
}

void showDisplayLine(uint8_t data[])
{
    char mydata[MQTT_LONG_LENGTH];
    bool array_match = true;

    sprintf(mydata, "-> %d-%d-%d-%d-%d-%d-%d-%d", checkBitValue(data[0]), checkBitValue(data[2]), checkBitValue(data[4]),
            checkBitValue(data[6]), checkBitValue(data[8]), checkBitValue(data[10]),
            checkBitValue(data[12]), checkBitValue(data[14]));
    //printf("showDisplayLine\n");
    printf("%s\n", mydata);
}

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
        printf("Header: %s\n", mydata);

        //TODO clean Lane Data
        //nein es werden immer alle Zeiten bis zum erbrechen geschickt ...
        for (int clearnr = 0; clearnr < DISPLAY_LANE_COUNT; clearnr++)
        {
            sprintf(COLORADO_HEAT_DATA[clearnr], "000000");
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
    hundredth = timehundredth(data);

    sprintf(mydata, "time %d%d:%d%d,%d", checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]));

    mqtt_send(mydata);
#ifdef debug_time
    printf("%s\n", mydata);
#endif
    return true;
};

int timehundredth(uint8_t data[])
{

    int minutes;
    minutes = checkBitValue(data[4]) * 10 + checkBitValue(data[6]);

    int seconds;
    seconds = checkBitValue(data[8]) * 10 + checkBitValue(data[10]);

    int decent;
    decent = checkBitValue(data[12]);
#ifdef debug_time
    printf("Minutes: %d Seconds: %d Zentel: %d\n", minutes, seconds, decent);
#endif
    int timehundredth = (minutes * 60 + seconds) * 100 + decent * 10;
#ifdef debug_time
    printf("timehundredth: %d\n", timehundredth);
#endif
    return timehundredth;
}

void debugLane(int lane, char shortdata[], uint8_t data[])
{
    if (lane == 0x01)
    {
        printf("change lane %d \n", lane);
        printf("change data %s \n", shortdata);
        printf("stored data %s \n", COLORADO_HEAT_DATA[lane - 1]);
        for (int i = 0; i < BUFFER_LENGTH; i++)
        {
            printf("%02x ", data[i]);
        }
        printf("\n");
    }
}