#include <cstdio>
#include <cstring>

#include "raw_connect.h"
#include <mosquittopp.h>

raw_connect::raw_connect(const char *id, const char *host, int port) : mosquittopp(id)
{
	int keepalive = 60;

	/* Connect immediately. This could also be done by calling
	 * mqtt_tempconv->connect(). */
	connect(host, port, keepalive);
};

raw_connect::~raw_connect()
{
}

void raw_connect::on_connect(int rc)
{
	printf("Connected with code %d.\n", rc);
	if(rc == 0){
		/* Only attempt to subscribe on a successful connect. */
		subscribe(NULL, "temperature/celsius");
	}
}
