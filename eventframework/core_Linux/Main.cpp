/*
 * Main.cpp
 *
 *  Created on: Jan 7, 2012
 *      Author: rene
 */

#include "ClientSub.h"

int main() {

	int rc;
	ClientSub *mqttc = new ClientSub("cppclientsub");

	mqttc->lib_init();

	mqttc->connect("test.mosquitto.org", 1883, 60, true);

	do{
		rc = mqttc->loop(-1);
	}while(rc == MOSQ_ERR_SUCCESS);

	mqttc->disconnect();
	mqttc->lib_cleanup();

}
