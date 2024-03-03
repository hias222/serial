#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include "serialUtils.h"
#include "mqttUtils.h"
#include "helperFunctions.h"

#define debug
//#define debug_trace

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#define DISPLAY_LANE_COUNT 10
#define BUFFER_LENGTH 16

#define MQTT_MESSAGE_LENGTH 16
// for the message to broker
#define MQTT_LONG_LENGTH 25

//char COLORADO_HEADER_DATA[MQTT_LONG_LENGTH];
//char **COLORADO_HEAT_DATA;
//char COLORADO_HEAT_DATA[DISPLAY_LANE_COUNT][MQTT_MESSAGE_LENGTH];
//char **COLORADO_PLACE_DATA;
//char COLORADO_PLACE_DATA[DISPLAY_LANE_COUNT][3];

char *mydata;
char *storeRoundsData;
int p_round;

//int noworking;
//bool running, stopping
//, pending;
//int hundredth;

int initanalyseData()
{
    printf("serialconnect - Load analyse data\n");
    //char *str;
    //str = (char *)malloc(sizeof(*str) * COLORADO_CHANNELS);

    int i, o;
    //hundredth = 0;
    //running = false;

    //p_round = malloc(cnt * sizeof *p_round);
    p_round = 0;

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
    mqtt_clean();
    return 0;
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
        p_round = 0;
        if (strcmp(mydata, storeRoundsData) != 0)
        {
            strcpy(storeRoundsData, mydata);
            sprintf(sendData, "round %d", p_round);
            mqtt_send(sendData);
        }
    }

    if (strcmp(mydata, storeRoundsData) != 0)
    {
        p_round++;
        strcpy(storeRoundsData, mydata);
        sprintf(sendData, "round %d", p_round);
        mqtt_send(sendData);
    }
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
