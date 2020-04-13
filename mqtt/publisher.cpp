#include "publisher.h"
#include <string>
#include <cstring>
#include "mosquitto.h"

publisher::publisher(const char *id, const char *host, const char *topic, int port)
{
    mqtthost = new char[strlen(host) + 1];
    strcpy(mqtthost, host);

    mqtttopic = new char[strlen(topic) + 1];
    strcpy(mqtttopic, topic);

    mqttid = new char[strlen(id)];
    strcpy(mqttid, id);

    mqttport = port;

    connect();
    //char *mqttmessage = (char *)malloc(100);
}

publisher::~publisher()
{
    mosquitto_lib_cleanup();
    printf("destroy");
};

int publisher::connect()
{
    int rc;
    bool clean_session = true;

    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, clean_session, NULL);

    if (!mosq)
    {
        fprintf(stderr, "Error: Out of memory.\n");
        return 1;
    }

    rc = mosquitto_connect(mosq,
                           mqtthost,
                           mqttport,
                           60);

    if (rc)
    {
        printf("Error: %s\n", mosquitto_strerror(rc));
        mosquitto_lib_cleanup();
        return rc;
    }

    return 0;
}

int publisher::publish(char *message)
{
    connect();
    printf("publish start \n");
    int *msgid;
    int rc;

    char mqttmessage[strlen(message) + 1];
    snprintf(mqttmessage, strlen(message) + 1, "%s", message);

    rc = mosquitto_publish(mosq,
                           msgid,
                           mqtttopic,
                           strlen(mqttmessage),
                           mqttmessage,
                           0,
                           0);

    if (rc)
    {
        printf("Error: %s\n", mosquitto_strerror(rc));
        return rc;
    }

    return 0;
}