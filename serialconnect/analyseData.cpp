#include <string>
#include <stdio.h>
#include <string.h>

#include "analyseData.h"
#include "serialUtils.h"
//#define debug_incoming
//#define debug_lane
#define COLORADO_CHANNELS 32
#define COLORADO_ADDRESS_WORD_MASK 0x80
#define COLORADO_ROWS 16

#define DISPLAY_LANE_COUNT 8

#define MQTT_MESSAGE_LENGTH 8
// for the message to broker
#define MQTT_LONG_LENGTH 25

uint8_t colorado_start_detected;
uint8_t buf[9];
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
    printf("initReadData");
    //char *str;
    //str = (char *)malloc(sizeof(*str) * COLORADO_CHANNELS);

    int i, o, ncolumns = 2, nrows = 3;

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
    return 0;
}

int cleanReadData()
{
    cleananalyseData();
    free(colorado_data);
    return 0;
}

int putReadData(uint8_t ReadData)
{
    uint8_t j;

    if ((ReadData & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
    {
#ifdef debug_incoming
        printf("start \n ");
#endif

        if (0x01 == colorado_start_detected)
        {
            if (in_count == colorado_channel_length[colorado_control_channel])
            {
                printf("save %d ctrl %02x \n", in_count, colorado_control_channel);

                for (j = 1; j < colorado_channel_length[colorado_control_channel]; j++)
                {
                    colorado_digit_no = (buf[j] >> 4) & 0x07;
#ifdef debug_incoming
                    printf("%d: [%d][%d] %d ", j, colorado_control_channel, (colorado_digit_no << 1) + colorado_control_bit, (~buf[j]) & 0x0F);
#endif
                    colorado_data[colorado_control_channel][(colorado_digit_no << 1) + colorado_control_bit] = (~buf[j]) & 0x0F;
                }

#ifdef debug_incoming
                printf("\n ");
#endif

                if (colorado_control_channel != 0x00 && colorado_control_channel < (DISPLAY_LANE_COUNT + 1))
                {
#ifdef debug_lane
                    printf("--- lane %02x \n", ReadData);
#endif
                    analyseActiveData(colorado_control_channel, colorado_data[colorado_control_channel]);
                }
                else if (colorado_control_channel == 0x00)
                {
                    checkStartStop(colorado_data[colorado_control_channel]);
                    loop++;
#ifdef debug_incoming
                    printf("--- time %02x \n", ReadData);
#endif
                    if (loop > 100)
                    {
                        printf("--- time %02x \n", ReadData);
                        loop = 0;
                    }
                }
                else
                {
#ifdef debug_lane
                    printf("--- update header %02x \n", ReadData);
#endif
                    getHeader(colorado_data[colorado_control_channel]);
                }
            }
#ifdef debug_incoming
            else
            {
                printf("not save %d ctrl %02x \n", in_count, colorado_control_channel);
            }
#endif
        }
        in_count = 1;
        colorado_start_detected = 0x01;
        buf[0] = ReadData;
        colorado_control_bit = buf[0] & 0x01;
        colorado_control_channel = (~(buf[0] >> 1)) & 0x1F;
#ifdef debug_incoming
        printf("colorado_control_bit: %#02x %#02x  \n", ReadData, colorado_control_bit);
        printf("colorado_control_channel: %#02x %#02x  \n", ReadData, colorado_control_channel);
        printf("we need %d bytes\n", colorado_channel_length[colorado_control_channel]);
#endif
    }
    else
    {
        if (0x01 == colorado_start_detected)
        { // wir hatten ein Adress Word erkannt => Daten speichern
            buf[in_count] = ReadData;
            in_count++;
            //printf("store %02x  \n", ReadData);

            if (in_count > 9)
            { // Ups... Da ist was schief gelaufen. Mehr als 8 Bytes bis zum nächsten Adress Word
                colorado_start_detected = 0x00;
#ifdef debug_incoming
                printf("uups %02x in_count too large \n", ReadData);
#endif
            }
        }
    }

    return true;
}
