#include <stdio.h>
#include <string>

#include "Base.hh"
#include "raw_connect.h"
#include "mqttpublisher.h"
#include "mosquitto.h"

int send_message()
{
    int rc;

    class mqttpublisher *publish;
    publish = new mqttpublisher("test", "localhost", "raw", 1883);

    char buf[50];
    sprintf(buf, "%s", "Nachricht eins");

    publish->connect();
    publish->publish(buf);

    sprintf(buf, "%s", "Nachricht zwei");

    for (int i = 0; i < 5; i++)
    {
        publish->publish(buf);
    }

    publish->~mqttpublisher();
    //delete publish;

    return 0;
}

int main()
{
    TEST_ASSERT_THROW(send_message() == 0);
};