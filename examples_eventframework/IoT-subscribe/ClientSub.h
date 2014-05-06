/*
 * ClientSub.h
 *
 *  Created on: Jan 11, 2012
 *      Author: rene
 */

#include <mosquittopp.h>
#include <iostream>
#ifndef CLIENTSUB_H_
#define CLIENTSUB_H_

class ClientSub : public mosquittopp::mosquittopp {

public:
	ClientSub(const char* id);
	virtual ~ClientSub();

private:
	uint16_t mid;

	void on_connect(int rc);
	void on_subscribe(uint16_t mid, int qos_count, const uint8_t *granted_qos);
	void on_message(const struct mosquitto_message *message);
	void print_error_connection(int rc);
};

#endif /* CLIENTSUB_H_ */
