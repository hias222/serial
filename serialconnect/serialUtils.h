#include <string>

bool analyseActiveData(uint8_t channel, uint8_t data[32]);
int getTime(uint8_t lane, uint8_t data[]);
void getHeader(uint8_t data[]);
void showDisplayLine(uint8_t data[]);
void storeRounds(uint8_t data[]);
int8_t checkBitValue(int8_t data);
bool checknotnull(uint8_t data[]);
bool checkStartStop(uint8_t data[]);
int initanalyseData();
int cleananalyseData();
bool getTime(uint8_t data[]);
int timehundredth(uint8_t data[]);
void restTimeAndplace(uint8_t lane);
void debugLane(int lane, char shortdata[], uint8_t data[]);
