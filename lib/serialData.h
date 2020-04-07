// C library headers
#include <stdio.h>
#include <string.h>
#include <string>

#define COLORADO_CHANNELS 32
#define COLORADO_ADDRESS_WORD_MASK 0x80
#define COLORADO_ROWS 16
#define DISPLAY_LANE_COUNT 8

class serialData
{
    uint8_t colorado_start_detected;
    uint8_t buf[9];
    uint8_t colorado_control_update;
    uint8_t colorado_data[COLORADO_CHANNELS][COLORADO_ROWS];
    uint8_t colorado_control_channel;
    uint8_t colorado_control_bit;
    uint8_t in_count ;
    uint8_t colorado_digit_no;
    uint8_t new_race_started;
    int noworking;
    int loop;
    bool running, stopping, pending;

public:
    bool analyseActiveData(uint8_t channel, uint8_t data[32]);
    int8_t checkBitValue(int8_t data);
    //serialData();
    int setstart();
    bool putData(uint8_t ReadData);
    bool checkStartStop(uint8_t data[32]);
    bool checknotnull (uint8_t data[32]);
};