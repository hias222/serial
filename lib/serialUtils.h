#include <string>

bool analyseActiveData(uint8_t channel, uint8_t data[32]);
int getTime(int lane, uint8_t data[]);
int8_t checkBitValue(int8_t data);
bool checknotnull(uint8_t data[]);
bool checkStartStop(uint8_t data[]);