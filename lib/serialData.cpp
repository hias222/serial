// C library headers
#include <stdio.h>
#include <string.h>

//#define debug_incoming
#define INFO_MQTT

#include "serialData.h"

#define COLORADO_CHANNELS 32
#define COLORADO_ADDRESS_WORD_MASK 0x80
#define COLORADO_ROWS 16

#define MQTT_MESSAGE_LENGTH 8
// for the message to broker
#define MQTT_LONG_LENGTH 25

serialData::serialData()
{
  colorado_start_detected = 0;
  loop = 0;
  in_count = 0;
  colorado_digit_no = 0;
  running = false;
  stopping = false;
  new_race_started = 0x00;
  noworking = 0;
  pending = false;
  publish = new mqttpublisher("test", "localhost", "raw", 1883);
  publish->connect();

  for (int i; i < DISPLAY_LANE_COUNT; i++)
  {
    for (int g; g < DISPLAY_LANE_COUNT; g++)
    {
      COLORADO_HEAT_DATA[i][g] = '\0';
    }
  }
  for (int i; i < DISPLAY_LANE_COUNT; i++)
  {
    for (int g; g < 2; g++)
    {
      COLORADO_PLACE_DATA[i][g] = '\0';
    }
  }
}

int serialData::setstart()
{
  return 1;
}

bool serialData::putData(uint8_t ReadData)
{
  const uint8_t colorado_channel_length[COLORADO_CHANNELS] = {7, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 0, 9, 0, 9, 9, 0, 9, 9, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0};
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
#ifdef debug_incoming
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

          //checkStartStop(colorado_data[colorado_control_channel]);
        }
        else
        {
#ifdef debug_incoming
          printf("--- update header %02x \n", ReadData);
#endif
          analyseActiveData(colorado_control_channel, colorado_data[colorado_control_channel]);
        }
      }
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
        printf("uups %02x in_count too large \n", ReadData);
      }
    }
  }

  return true;
}

bool serialData::analyseActiveData(uint8_t channel, uint8_t data[32])
{
  //MQTT_LONG_LENGTH
  char mydata[64];

#ifdef debug_incoming
  printf("data: 0: %#02x  1: %#02x  2: %#02x 3: %#02x 4: %#02x 5: %#02x 6: %#02x 7: %#02x 8: %#02x \n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7], buf[8]);
  sprintf(mydata, "channel: %d: 2: %d 4: %d 6: %d 8: %d 10: %d 12: %d 14: %d", channel, checkBitValue(data[2]), checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]), data[10], data[12], data[14]);
  printf("result: %s \n", mydata);
#endif

  sprintf(mydata, "%d: %d %d%d %d%d %d%d", channel, checkBitValue(data[2]), checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]), data[10], data[12], data[14]);
  printf("%s \n", mydata);

  getTime(channel, data);

  return true;
}

int serialData::getTime(int lane, uint8_t data[])
{
  char mydata[MQTT_LONG_LENGTH];
  char shortdata[MQTT_MESSAGE_LENGTH] = "0000000";
  char log[MQTT_MESSAGE_LENGTH];
  char place[2];
  bool array_match = false;
  publish->publish("getTime");

  publish->publish("---");
  publish->publish(shortdata);
  publish->publish("---");
  printf(shortdata);

  // Button ZEit
  //Problem ees wird die ZEit aus dem letzten lauf geschickt mit 0 als platz
  sprintf(shortdata, "%d%d%d%d%d%d", checkBitValue(data[4]), checkBitValue(data[14]), checkBitValue(data[12]), checkBitValue(data[10]), checkBitValue(data[8]), checkBitValue(data[6]));
  //sprintf(shortdata, "%d%d%d%d%d%d", checkBitValue(data[2]), checkBitValue(data[14]), checkBitValue(data[12]), checkBitValue(data[10]), checkBitValue(data[8]), checkBitValue(data[6]));
  sprintf(mydata, "lane %d %d%d:%d%d,%d%d %d", lane, checkBitValue(data[4]), checkBitValue(data[6]), checkBitValue(data[8]), checkBitValue(data[10]), checkBitValue(data[12]), checkBitValue(data[14]), checkBitValue(data[2]));
  sprintf(place, "%d", checkBitValue(data[2]));

  publish->publish("---");
  publish->publish(shortdata);
  publish->publish(mydata);
  publish->publish(mydata);
  publish->publish(place);
  publish->publish("---");

  if (strcmp(shortdata, COLORADO_HEAT_DATA[lane - 1]) == 0)
  {
    publish->publish("#");
  }

  /*
  if (strcmp(shortdata, COLORADO_HEAT_DATA[lane - 1]) == 0)
  {
    array_match = true;
    
    if (strcmp(place, COLORADO_PLACE_DATA[lane - 1]) != 0)
    {

      //der platz ist ungleich
      if (strcmp(place, "0") != 0)
      {
        //jetzt müssen wir schicken
        array_match = false;
      }
    }
    
  }
  else
  {

    if (strcmp(shortdata, "000000") == 0)
    {
      array_match = true;
    }
    else
    {
      array_match = false;
    }
  }
  */

  if (checkBitValue(data[2]) == 0 && checkBitValue(data[14]) == 13)
  {
    array_match = true;
  }

  if (!array_match)
  {
    //publish->publish(mydata);

    for (int i = 0; i < MQTT_MESSAGE_LENGTH; i++)
    {
      COLORADO_HEAT_DATA[lane - 1][i] = shortdata[i];
    }

    for (int i = 0; i < 2; i++)
    {
      COLORADO_PLACE_DATA[lane - 1][i] = place[i];
    }
  }
  return 0;
}

int8_t serialData::checkBitValue(int8_t data)
{
  if (data == 0x0F)
  {
    return 0x00;
  }
  return data;
};

bool serialData::checknotnull(uint8_t data[])
{
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

bool serialData::checkStartStop(uint8_t data[])
{
  running = checknotnull(data);

  if (stopping != running)
  {
    if (running)
    {
      //coloradoraw.publish("start");
      new_race_started = 0x01;

#ifdef INFO_MQTT
      //coloradodebug.publish("Start ---->");
      printf("Start ---->\n");
#endif
    }
    else
    {
      //coloradoraw.publish("stop");
#ifdef INFO_MQTT
      //coloradodebug.publish("Stop ---->");
      printf("Stop ---->\n");
#endif
    }
  }
  stopping = running;
  return true;
};