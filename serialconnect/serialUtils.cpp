#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"

#define debug
#define debug_trace

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
char **COLORADO_HEAT_DATA;
//char COLORADO_HEAT_DATA[DISPLAY_LANE_COUNT][MQTT_MESSAGE_LENGTH];
char **COLORADO_PLACE_DATA;
//char COLORADO_PLACE_DATA[DISPLAY_LANE_COUNT][3];

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

    int i, o;
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

    COLORADO_HEAT_DATA = (char **)malloc(sizeof(char *) * DISPLAY_LANE_COUNT);
    for (i = 0; i < DISPLAY_LANE_COUNT; i++)
    {
        COLORADO_HEAT_DATA[i] = (char *)malloc(sizeof(char) * MQTT_MESSAGE_LENGTH);
    }

    COLORADO_PLACE_DATA = (char **)malloc(sizeof(char *) * DISPLAY_LANE_COUNT);
    for (i = 0; i < DISPLAY_LANE_COUNT; i++)
    {
        COLORADO_PLACE_DATA[i] = (char *)malloc(sizeof(char) * 3);
    }

    return 0;
}

int cleananalyseData()
{
    mqtt_clean();
    return 0;
}

void getLaneTime(uint8_t lane, uint8_t data[])
{
    //char mydata[MQTT_LONG_LENGTH];
    char shortdata[MQTT_MESSAGE_LENGTH] = "0000000";
    char log[MQTT_MESSAGE_LENGTH];
    char place[3];
    bool array_match = false;
    char nullplace[3] = "0";

    // Button ZEit
    //Problem ees wird die Zeit aus dem letzten lauf geschickt mit 0 als platz
    sprintf(shortdata, "%d%d%d%d%d%d", checkBitValue(data[4]), checkBitValue(data[14]), checkBitValue(data[12]), checkBitValue(data[10]), checkBitValue(data[8]), checkBitValue(data[6]));
    sprintf(mydata, "lane %d %d%d:%d%d,%d%d %d", lane, checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]), checkBitValue(data[2]));
    sprintf(place, "%d", checkBitValue(data[2]));

#ifdef debug_trace
    printf("getLaneTime - step 1\n");
#endif

    if (strcmp(shortdata, COLORADO_HEAT_DATA[lane - 1]) == 0)
    {
#ifdef debug_trace
        printf("getLaneTime - step 1a\n");
#endif
        array_match = true;
        if (strcmp(place, COLORADO_PLACE_DATA[lane - 1]) != 0)
        {
            //der platz ist ungleich 0
#ifdef debug_trace
            printf("getLaneTime - step 1aa\n");
#endif
            if (strcmp(place, nullplace) != 0)
            {
#ifdef debug_trace
                printf("getLaneTime - step 1aaa\n");
#endif
                //jetzt müssen wir schicken
                array_match = false;
            }
        }
    }
    else
    {
#ifdef debug_trace
        printf("getLaneTime - step 1b\n");
#endif
        if (strcmp(shortdata, "000000") == 0)
        {
#ifdef debug_trace
            printf("getLaneTime - step 1bb\n");
#endif
            array_match = true;
            restTimeAndplace(lane);
        }
        else
        {
#ifdef debug_trace
            printf("getLaneTime - step 1bc\n");
#endif
            array_match = false;
        }
    }

#ifdef debug_trace
    printf("getLaneTime - step 2\n");
#endif

    // ???
    if (checkBitValue(data[2]) == 0 && checkBitValue(data[14]) == 13)
    {
#ifdef debug_trace
        printf("getLaneTime - step 2a\n");
#endif
        array_match = true;
    }

#ifdef debug_trace
    printf("getLaneTime - step 3\n");
#endif

    if (!array_match)
    {
        array_match = true;
        // wir schicken nur wenn die uhr läuft
        if (hundredth > 0)
        {
#ifdef debug_trace
            printf("getLaneTime - step 3a\n");
#endif
            if (mqtt_send(mydata))
            {
#ifdef debug_trace
                printf("getLaneTime - step 3a\n");
#endif
                printf("Error sending \n");
            }
        }
#ifdef debug_trace
        printf("getLaneTime - step 3b\n");
#endif

        for (int i = 0; i < MQTT_MESSAGE_LENGTH; i++)
        {
            COLORADO_HEAT_DATA[lane - 1][i] = shortdata[i];
        }

#ifdef debug_trace
        printf("getLaneTime - step 3c\n");
#endif

        for (int i = 0; i < 3; i++)
        {
            COLORADO_PLACE_DATA[lane - 1][i] = place[i];
        }
    }

#ifdef debug_trace
    printf("getLaneTime - step 4\n");
#endif
}

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

void storeRoundsInternal(uint8_t data[])
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

void getHeaderInternal(uint8_t data[])
{
    char mydata[MQTT_LONG_LENGTH];
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

void checkStartStopInternal(uint8_t data[])
{
    running = checknotnull(data);
    char mydata[MQTT_LONG_LENGTH];

    if (stopping != running)
    {
        if (running)
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
    stopping = running;
};

void getTimeInternal(uint8_t data[])
{
    char mydata[MQTT_LONG_LENGTH];
    running = checknotnull(data);
    hundredth = timehundredth(data);

    sprintf(mydata, "time %d%d:%d%d,%d", checkBitValue(data[4]), checkBitValue(data[6]),
            checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]));

    mqtt_send(mydata);
};

int timehundredth(uint8_t data[])
{

    int minutes;
    minutes = checkBitValue(data[4]) * 10 + checkBitValue(data[6]);

    int seconds;
    seconds = checkBitValue(data[8]) * 10 + checkBitValue(data[10]);

    int decent;
    decent = checkBitValue(data[12]);
    int timehundredth = (minutes * 60 + seconds) * 100 + decent * 10;

    return timehundredth;
}

void analyseActiveData(uint8_t channel, uint8_t *data[])
{
#ifdef debug
    printf("analyseActiveData - start\n");
#endif
    getLaneTime(channel, *data);

#ifdef debug
    printf("analyseActiveData - end\n");
#endif
}

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

void storeRounds(uint8_t *data[])
{
#ifdef debug
    printf("storeRounds - start\n");
#endif
    storeRoundsInternal(*data);
#ifdef debug
    printf("storeRounds - end\n");
#endif
}
