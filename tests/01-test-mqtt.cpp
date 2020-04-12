#include <stdio.h>
#include <string>

#include "Base.hh"
#include "raw_connect.h"
#include "publisher.h"
#include "mosquitto.h"

int main()
{
    int rc;

    mosquitto_lib_init();

    class publisher *publish;
    publish = new publisher("test", "localhost", "raw", 1883);

    char buf[50];
    //buf=(char)malloc(60*sizeof(char));
    sprintf(buf,"%s", "h123456789 ello");

    publish->connect();
    TEST_ASSERT_THROW(publish->publish(buf) == 0);

    char buf2[50];
    publish->connect();

    sprintf(buf2,"%s", "end");
    TEST_ASSERT_THROW(publish->publish(buf2) == 0);
    
    mosquitto_lib_cleanup();

    return 0;
};