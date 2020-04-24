#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <cstring>
#include "mosquitto.h"

int raw_connect()
{
	int rc;
	struct mosquitto_message *msg;
	struct mosquitto *mosq;

	bool clean_session = true;

	int *msgid;
	char buf[50];
	snprintf(buf, 50, "%s", "Hello World");

	mosquitto_lib_init();
	mosq = mosquitto_new(NULL, clean_session, NULL);

	if (!mosq)
	{
		fprintf(stderr, "Error: Out of memory.\n");
		mosquitto_lib_cleanup();
		return 1;
	}

	rc = mosquitto_connect(mosq,
						   "localhost",
						   1883,
						   60);

	if (rc)
	{
		printf("Error: %s\n", mosquitto_strerror(rc));
		mosquitto_lib_cleanup();
		return rc;
	}

	//printf("%s %s\n", msg->topic, (char *)msg->payload);

	printf("publish");

	rc = mosquitto_publish(mosq,
						   msgid,
						   "rawdata",
						   strlen(buf),
						   buf,
						   0,
						   0);
	if (rc)
	{
		printf("Error: %s\n", mosquitto_strerror(rc));
		mosquitto_lib_cleanup();
		return rc;
	}

	mosquitto_lib_cleanup();

	return 0;
}
