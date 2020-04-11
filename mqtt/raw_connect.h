#ifndef TEMPERATURE_CONVERSION_H
#define TEMPERATURE_CONVERSION_H

#include <mosquittopp.h>

class raw_connect : public mosqpp::mosquittopp
{
	public:
		raw_connect(const char *id, const char *host, int port);
		~raw_connect();

		void on_connect(int rc);
		
};

#endif
