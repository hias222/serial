#include "mosquitto.h"

class mqttpublisher
{

public:
    mqttpublisher(const char *id, const char *host, const char *topic, int port);
    ~mqttpublisher();

    int connect();
    int publish(char * message);

    struct mosquitto *mosq;
    char* mqtthost, *mqttid, *mqtttopic;
    int mqttport;

    char* mqttmessage;
};