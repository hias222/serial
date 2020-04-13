#include <iostream>

#include "listener.h"
#include "analyseData.h"

int main(int, char**) {
    std::cout << "Serial start!\n";
    //startListen();
     //char readData = { 0x80 };

    printf("Start testSerialData ..\n");


    //myData.setstart();
    printf("lane\n");
    putReadData(0xb0);
    printf("lane data\n");
    putReadData(0x3);
    putReadData(0x1f);
    putReadData(0x2f);
    putReadData(0x49);
    putReadData(0x56);
    putReadData(0x5d);
    putReadData(0x6f);
    putReadData(0x7f);
    // next
    printf("\time\n");
    putReadData(0xbe);
    printf("time data\n");
    putReadData(0xbe);
    putReadData(0xF);
    putReadData(0x1F);
    putReadData(0x2f);
    putReadData(0x39);
    putReadData(0x4c);
    putReadData(0x5d);
    //empty
    for (int i = 0; i < 120; i++)
    {
        putReadData(0xbe);
        putReadData(0xF);
        putReadData(0x1F);
        putReadData(0x2f);
        putReadData(0x3f);
        putReadData(0x4f);
        putReadData(0x5f);
    }
    //loop 0x0F
    for (int i = 0; i < 120; i++)
    {
        putReadData(0xbe);
        putReadData(0xF);
        putReadData(0x1F);
        putReadData(0x2f);
        putReadData(0x39);
        putReadData(0x4c);
        putReadData(0x5d);
    }
    //empty
    for (int i = 0; i < 120; i++)
    {
        putReadData(0xbe);
        putReadData(0xF);
        putReadData(0x1F);
        putReadData(0x2f);
        putReadData(0x3f);
        putReadData(0x4f);
        putReadData(0x5f);
    }

    //next
    printf("header\n");
    putReadData(0x80);
    /*
    printf("time data\n");
    //todo
    putReadData(0x38);
    putReadData(0x39);
    putReadData(0x38);
    putReadData(0x07);
    putReadData(0x07);
    putReadData(0x07);
    putReadData(0x07);
    putReadData(0x07);
    putReadData(0x07);
    putReadData(0x07);

    //more
    putReadData(0x80);
    */

}
