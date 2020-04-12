#include <stdio.h>
#include <string>

#include "Base.hh"
#include "raw_connect.h"
#include "publisher.h"
#include "mosquitto.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

int main()
{
    int rc;
    char buf[51];

    mosquitto_lib_init();

    class publisher *publish;
    publish = new publisher("test", "localhost","raw", 1883);
    //publish->connect();

    TEST_ASSERT_THROW(publish->connect() == 0);
    TEST_ASSERT_THROW(publish->publish("Waitng 20 seconds") == 0);

    sleep(20);

    /*
    for (int i=0; i<500; i++){
        sprintf(buf,"%d: Hello World",i);
        publish->publish(buf);
    }
    */

    TEST_ASSERT_THROW(publish->publish("Hello End") == 0);

    //TEST_ASSERT_THROW(raw_connect() == 0);

    mosquitto_lib_cleanup();

    return 0;
};