#include <string>
#include <stdio.h>
#include <string.h>

#include "analyseData.h"
#include "serialUtils.h"
#include "helperFunctions.h"
#include "analyseStartStop.h"
#include "analyseHeader.h"
#include "analyseRunningTime.h"
#include "analyseLane.h"

#define debug

//#define debug_lane_pointer

#define LOOP_COUNT_DEF 10
#define COLORADO_CHANNELS 32
#define COLORADO_ADDRESS_WORD_MASK 0x80
#define COLORADO_ROWS 16

#define DISPLAY_LANE_COUNT 10
#define BUFFER_LENGTH 10

#define MQTT_MESSAGE_LENGTH 16
// for the message to broker
#define MQTT_LONG_LENGTH 25

uint8_t colorado_start_detected;
uint8_t buf[BUFFER_LENGTH];
uint8_t colorado_control_update;
uint8_t **colorado_data;
//uint8_t colorado_data[COLORADO_CHANNELS][COLORADO_ROWS];
uint8_t colorado_control_channel;
uint8_t colorado_control_bit;
uint8_t in_count;
uint8_t colorado_digit_no;

const uint8_t colorado_channel_length[COLORADO_CHANNELS] = {7, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 9, 0, 9, 9, 0, 9, 9, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int loop;

int initReadData()
{
    printf("serialconnect - Load timing data\n");

    int i, o, ncolumns = 2, nrows = 3;

    //only way to alloc 2 dimensonal

    colorado_data = (uint8_t **)malloc(sizeof(uint8_t *) * 32);
    for (i = 0; i < 32; i++)
    {
        colorado_data[i] = (uint8_t *)malloc(sizeof(uint8_t) * 16);
    }

    for (o = 0; o < 32; o++)
    {
        for (i = 0; i < 16; i++)
        {
            colorado_data[o][i] = 0x01;
        }
    }

// more inits

    initanalyseData();
    initRunninTime();
    initanalyseLane(DISPLAY_LANE_COUNT, MQTT_MESSAGE_LENGTH);
    initanalyseHeader(MQTT_LONG_LENGTH);
#ifdef debug
    printf("analyseData - INIT reaady\n");
#endif
    return 0;
}

int cleanReadData()
{
    cleananalyseData();
    cleananalyseLane();
    free(colorado_data);
    return 0;
}

int putReadData(uint8_t ReadData)
{
    uint8_t j;

    if ((ReadData & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
    {

        if (0x01 == colorado_start_detected)
        {
            if (in_count == colorado_channel_length[colorado_control_channel])
            {

                for (j = 1; j < colorado_channel_length[colorado_control_channel]; j++)
                {
                    colorado_digit_no = (buf[j] >> 4) & 0x07;
                    colorado_data[colorado_control_channel][(colorado_digit_no << 1) + colorado_control_bit] = (~buf[j]) & 0x0F;
                }

                if (colorado_control_channel != 0x00 && colorado_control_channel < (DISPLAY_LANE_COUNT + 1))
                {

#ifdef debug_lane_pointer
                    showDisplayLine(&colorado_data[colorado_control_channel]);
#endif

                    //please check number lanes in colorado config !!!!!!!!!
                    analyseActiveData(colorado_control_channel, &colorado_data[colorado_control_channel]);
                }
                else if (colorado_control_channel == 0x00)
                {
                    checkStartStop(&colorado_data[colorado_control_channel]);
                    loop++;
                    // alle 10 anschauen
                    if (loop > LOOP_COUNT_DEF)
                    {
                        getTime(&colorado_data[colorado_control_channel]);
                        loop = 0;
                    }
                }
                else if (colorado_control_channel == 0x0c)
                {
#ifdef debug_header
                    // https://www.coloradotime.com/manuals/System_6_Swimming_Manual_F890.pdf
                    //0c
                    // 0c oder 0a
                    // A600102E304050607D
                    //outPutBuffer(colorado_control_channel, buf);
#endif
                    getHeader(&colorado_data[colorado_control_channel]);
                }
                else if (colorado_control_channel == 0x12)
                {
                    storeRounds(&colorado_data[colorado_control_channel]);
                }
            }
        }
        in_count = 1;
        colorado_start_detected = 0x01;
        buf[0] = ReadData;
        colorado_control_bit = buf[0] & 0x01;
        colorado_control_channel = (~(buf[0] >> 1)) & 0x1F;
    }
    else
    {
        if (0x01 == colorado_start_detected)
        { // wir hatten ein Adress Word erkannt => Daten speichern
            buf[in_count] = ReadData;
            in_count++;
            if (in_count > 9)
            { // Ups... Da ist was schief gelaufen. Mehr als 8 Bytes bis zum nächsten Adress Word
                colorado_start_detected = 0x00;
            }
        }
    }

    return true;
}

/*
int outPutBuffer(int code, uint8_t data[])
{
    char mydata[64];

    printf("raw Buffer ");
    printf("%02x: ", code);
    for (int i = 0; i < BUFFER_LENGTH; i++)
    {
        //printf("%02x %d ", data[i], data[i]);
        printf("%02x ", data[i]);
    }
    printf("\n");
        sprintf(mydata, "channel: %d: 2: %d 4: %d 6: %d 8: %d 10: %d 12: %d 14: %d", code, checkBitValue(data[2]), checkBitValue(data[4]),
            checkBitValue(data[6]), checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]));
    printf("%s \n", mydata);

    return 0;
}
*/
