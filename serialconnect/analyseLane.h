#include <string>

void initanalyseLane(int laneCount, int mqqMsgLength);
void getLaneTime(uint8_t lane, uint8_t data[]);
void analyseActiveData(uint8_t channel, uint8_t *data[]);
void resetAllData();
void cleananalyseLane();