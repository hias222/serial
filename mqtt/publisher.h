#include "mosquitto.h"

class publisher
{

public:
    publisher(const char *id, const char *host, const char *topic, int port);
    ~publisher();

    int connect();
    int publish(char * message);

    struct mosquitto *mosq;
    char* mqtthost, *mqttid, *mqtttopic;
    int mqttport;

    char* mqttmessage;
};