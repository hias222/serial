#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"
#include <unistd.h>

#include "mqttUtils.h"

#define mqtt_host "localhost"
#define mqtt_port 1883

static int run = 1;
struct mosquitto *mosq;
char clientid[24];
uint8_t reconnect;

int mqtt_connect()
{
    reconnect = true;
    int rc = 0;

    mosquitto_lib_init();

    memset(clientid, 0, 24);
    snprintf(clientid, 23, "mysql_log_%d", getpid());
    mosq = mosquitto_new(clientid, true, 0);

    if (!mosq)
    {
        fprintf(stderr, "Error: Out of memory.\n");
        mosquitto_lib_cleanup();
        return 1;
    }
    return 0;
}

int mqtt_send(char message[MQTT_LONG_LENGTH])
{
    int rc;
    int *msgid;

    char buf[50];
    //snprintf(buf, 50, "%s", "Hello World");
    char mqttmessage[strlen(message) + 1];
    snprintf(mqttmessage, strlen(message) + 1, "%s", message);

    if (mosq)
    {

        rc = mosquitto_connect(mosq, mqtt_host, mqtt_port, 60);

        if (rc)
        {
            printf("Error: %s\n", mosquitto_strerror(rc));
            mosquitto_lib_cleanup();
            return rc;
        }

        rc = mosquitto_publish(mosq,
                               msgid,
                               "raw",
                               strlen(mqttmessage),
                               mqttmessage,
                               0,
                               0);

        if (rc)
        {
            printf("Error: %s\n", mosquitto_strerror(rc));
            mqtt_clean();
            return rc;
        }
        //mosquitto_destroy(mosq);
    }

    return 0;
}

int mqtt_clean()
{
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
};
