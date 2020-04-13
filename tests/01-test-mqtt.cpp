#include <stdio.h>
#include <string>

#include "Base.hh"
#include "raw_connect.h"
#include "publisher.h"
#include "mosquitto.h"

int main()
{
    int rc;

    class publisher *publish;
    publish = new publisher("test", "localhost", "raw", 1883);

    char buf[50];
    sprintf(buf, "%s", "Nachricht eins");

    publish->connect();
    TEST_ASSERT_THROW(publish->publish(buf) == 0);

    //publish->~publisher();

    //publish = new publisher("test", "localhost", "raw", 1883);
    sprintf(buf, "%s", "Nachricht zwei");

    for (int i = 0; i < 5 ; i++)
    {
        publish->connect();
        TEST_ASSERT_THROW(publish->publish(buf) == 0);
    }

    publish->~publisher();

    return 0;
};