#include <stdio.h>

#include "serialData.h"
#include "Base.hh"

int main()
{
    //char readData = { 0x80 };

    printf("Start testSerialData ..\n");

    serialData myData = serialData();
    myData.setstart();
    printf("lane\n");
    myData.putData(0xb0);
    printf("lane data\n");
    myData.putData(0x3);
    myData.putData(0x1f);
    myData.putData(0x2f);
    myData.putData(0x49);
    myData.putData(0x56);
    myData.putData(0x5d);
    myData.putData(0x6f);
    myData.putData(0x7f);
    // next
    printf("\time\n");
    myData.putData(0xbe);
    printf("time data\n");
    myData.putData(0xbe);
    myData.putData(0xF);
    myData.putData(0x1F);
    myData.putData(0x2f);
    myData.putData(0x39);
    myData.putData(0x4c);
    myData.putData(0x5d);
    //empty
    for (int i = 0; i < 120; i++)
    {
        myData.putData(0xbe);
        myData.putData(0xF);
        myData.putData(0x1F);
        myData.putData(0x2f);
        myData.putData(0x3f);
        myData.putData(0x4f);
        myData.putData(0x5f);
    }
    //loop 0x0F
    for (int i = 0; i < 120; i++)
    {
        myData.putData(0xbe);
        myData.putData(0xF);
        myData.putData(0x1F);
        myData.putData(0x2f);
        myData.putData(0x39);
        myData.putData(0x4c);
        myData.putData(0x5d);
    }
    //empty
    for (int i = 0; i < 120; i++)
    {
        myData.putData(0xbe);
        myData.putData(0xF);
        myData.putData(0x1F);
        myData.putData(0x2f);
        myData.putData(0x3f);
        myData.putData(0x4f);
        myData.putData(0x5f);
    }

    //next
    printf("header\n");
    myData.putData(0x80);
    /*
    printf("time data\n");
    //todo
    myData.putData(0x38);
    myData.putData(0x39);
    myData.putData(0x38);
    myData.putData(0x07);
    myData.putData(0x07);
    myData.putData(0x07);
    myData.putData(0x07);
    myData.putData(0x07);
    myData.putData(0x07);
    myData.putData(0x07);

    //more
    myData.putData(0x80);
    */

    TEST_ASSERT_THROW(false);

    return 0;
}