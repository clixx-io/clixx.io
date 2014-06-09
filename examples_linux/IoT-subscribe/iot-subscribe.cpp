/*
 * Main.cpp
 *
 *  Created on: Jan 7, 2012
 *      Author: rene
 */

#include "clixxIO.hpp"
#include <mosquittopp.h>
#include <iostream>

int main() {

	int rc;
	ClixxIO_IoTSub *mqttc = new ClixxIO_IoTSub("cppclientsub");

	mqttc->lib_init();

	mqttc->connect("test.mosquitto.org", 1883, 60, true);

	do{
		rc = mqttc->loop(-1);
	}while(rc == MOSQ_ERR_SUCCESS);

	mqttc->disconnect();
	mqttc->lib_cleanup();

}
