#include <string>
#include <stdio.h>
#include <string.h>

#include "analyseData.h"

#define COLORADO_ADDRESS_WORD_MASK 0x80

int putReadData(uint8_t ReadData)
{
    if ((ReadData & COLORADO_ADDRESS_WORD_MASK) == COLORADO_ADDRESS_WORD_MASK)
    {
        printf("\n");
    }
    printf("%02x ", ReadData);
    fflush(stdout);
    return 0;
}
