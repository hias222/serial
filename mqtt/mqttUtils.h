#define MQTT_LONG_LENGTH 25

int mqtt_connect(bool remoteserial);
int mqtt_clean();
int mqtt_send(char message[MQTT_LONG_LENGTH]);

