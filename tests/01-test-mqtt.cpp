#include <stdio.h>
#include <string>

#include "Base.hh"
#include "raw_connect.h"

int main()
{
    int rc;
    char buf[51];
    raw_connect();
    TEST_ASSERT_THROW(raw_connect() == 0);
    return 0;
}