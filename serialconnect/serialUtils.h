#include <string>

bool analyseActiveData(uint8_t channel, uint8_t *data[]);
int getTime(uint8_t lane, uint8_t data[]);
void getHeader(uint8_t data[]);
void storeRounds(uint8_t data[]);
uint8_t checkBitValue(uint8_t data);
bool checknotnull(uint8_t data[]);
bool checkStartStop(uint8_t data[]);
int initanalyseData();
int cleananalyseData();
bool getTime(uint8_t data[]);
int timehundredth(uint8_t data[]);
void restTimeAndplace(uint8_t lane);

