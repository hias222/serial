#include <string>
#include <stdio.h>
#include <string.h>

#include "analyseData.h"
#include "serialUtils.h"
//#define debug_incoming
//#define debug_lane
//#define debug_header
//#define debug_time
//#define debug_data_lanes
//#define debug_lane_pointer
//#define debug_store_rounds
//#define debug_data_others
//#define debug_data_start
#define LOOP_COUNT_DEF 30
#define COLORADO_CHANNELS 32
#define COLORADO_ADDRESS_WORD_MASK 0x80
#define COLORADO_ROWS 16

#define DISPLAY_LANE_COUNT 10
#define BUFFER_LENGTH 9

#define MQTT_MESSAGE_LENGTH 8
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
#ifdef debug_incoming
    printf("%02x ", ReadData);
#endif

    if ((ReadData & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
    {
#ifdef debug_incoming
        printf("start data \n ");
#endif

        if (0x01 == colorado_start_detected)
        {
            if (in_count == colorado_channel_length[colorado_control_channel])
            {

#ifdef debug_incoming
                printf("save %d ctrl %02x \n", in_count, colorado_control_channel);
#endif
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

#ifdef debug_data_lanes
                    outPutBuffer(colorado_control_channel, buf);
#endif

#ifdef debug_lane_pointer
                    printf("lane %d \n", colorado_control_channel);
                    showDisplayLine(colorado_data[colorado_control_channel]);
#endif

#ifdef debug_lane
                    outPutBuffer(colorado_control_channel, buf);
#endif
                    //please check number lanes in colorado config !!!!!!!!!
                    analyseActiveData(colorado_control_channel, &colorado_data[colorado_control_channel]);
                }
                else if (colorado_control_channel == 0x00)
                {
                    checkStartStop(colorado_data[colorado_control_channel]);
                    loop++;
#ifdef debug_incoming
                    printf("--- time %02x \n", ReadData);
                    outPutBuffer(colorado_control_channel, buf);

#endif
                    // alle 10 anschauen
                    if (loop > LOOP_COUNT_DEF)
                    {
                        getTime(colorado_data[colorado_control_channel]);
#ifdef debug_time
                        printf("--- time %02x \n", ReadData);
                        outPutBuffer(colorado_control_channel, buf);
#endif
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
                    outPutBuffer(colorado_control_channel, buf);
#endif
                    getHeader(colorado_data[colorado_control_channel]);
                }
                else if (colorado_control_channel == 0x12)
                {
#ifdef debug_store_rounds
                    printf("0d - store rounds \n ");
                    showDisplayLine(colorado_data[colorado_control_channel]);
#endif
                    storeRounds(colorado_data[colorado_control_channel]);
                }
#ifdef debug_data_others
                else
                {
                    if (colorado_control_channel > (DISPLAY_LANE_COUNT))
                    {
                        printf("others - %02x \n ", colorado_control_channel);
                        showDisplayLine(colorado_data[colorado_control_channel]);
                    }
                }
#endif
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
#ifdef debug_incoming
        printf("last\n");
        outPutBuffer(colorado_control_channel, buf);
#endif
        buf[0] = ReadData;
        colorado_control_bit = buf[0] & 0x01;
        colorado_control_channel = (~(buf[0] >> 1)) & 0x1F;
#ifdef debug_incoming
        printf("colorado_control_bit: %02x %#02x  \n", ReadData, colorado_control_bit);
        printf("colorado_control_channel: %02x %#02x  \n", ReadData, colorado_control_channel);
        printf("we need %d bytes\n", colorado_channel_length[colorado_control_channel]);
#endif
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
#ifdef debug_incoming
                printf("uups %02x in_count too large \n", ReadData);
#endif

#ifdef debug_data_start
                printf("reset colorado_start_detected \n ");
#endif
            }
        }
    }

    return true;
}

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

