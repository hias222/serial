#include <string>
#include <stdio.h>
#include <string.h>
#include "mosquitto.h"

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

#include "mqttUtils.h"

#define mqtt_host "localhost"
#define mqtt_port 1883

static int run = 1;
struct mosquitto *mosq;
char clientid[24];
uint8_t reconnect;
bool error_state = false;


//#define debug_send

#include "send.h"

int mqtt_connect()
{
    reconnect = true;
    int rc = 0;

    mosquitto_lib_init();

    memset(clientid, 0, 24);
#ifdef _WIN32
    snprintf(clientid, 23, "mysql_log_%d", 1);
#else
    snprintf(clientid, 23, "mysql_log_%d", getpid());
#endif

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
    //unsigned
    char *remotechar = (char *)malloc(strlen(message));
    memset(&remotechar[0], 0, sizeof(remotechar));

#ifdef debug_send
    printf("mqtt_send = %s\n", message);
#endif

    if (mosq)
    {
        if (error_state)
        {
            error_state = false;
            printf("Reconnect \n");
            mosquitto_reconnect(mosq);
        }

        rc = mosquitto_connect(mosq, mqtt_host, mqtt_port, 60);

        if (rc)
        {
            printf("mosquitto_connect \n");
            printf("Error: %s\n", mosquitto_strerror(rc));
            error_state = true;
            free(remotechar);
            return rc;
        }

        rc = mosquitto_publish(mosq,
                               0,
                               "rawdata",
                               strlen(message),
                               message,
                               0,
                               0);

        if (rc)
        {
            printf("Error: %s\n", mosquitto_strerror(rc));
            error_state = true;
            free(remotechar);
            return rc;
        }
        //mosquitto_destroy(mosq);
    }
    else
    {
        printf("no mqtt connection \n");
    }

    free(remotechar);
    return 0;
}

int mqtt_clean()
{
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();
    return 0;
};
