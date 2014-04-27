/*
 * ClientSub.cpp
 *
 *  Created on: Jan 11, 2012
 *      Author: rene
 */

#include "ClientSub.h"

using namespace std;

ClientSub::ClientSub(const char* id) : mosquittopp(id){

}

ClientSub::~ClientSub() {

}

void ClientSub::on_connect(int rc) {

	if (!rc) {
		subscribe(&mid, "#", 0);
	}
	else {
		print_error_connection(rc);
	}
}

void ClientSub::on_subscribe(uint16_t mid, int qos_count, const uint8_t *granted_qos) {

	cout << "Subscribed (mid: " << mid << "): " << granted_qos[0] << endl;
	for(int i=1; i<qos_count; i++){
		cout << ", " <<granted_qos[i] << endl;
	}
}

void ClientSub::on_message(const struct mosquitto_message *message) {

	if(message->payloadlen){	// message contains a payload
			cout << message->topic << ": " << message->payload << endl;
	} else {
		cout << message->topic << ": (null)" << endl;
	}
}

void ClientSub::print_error_connection(int rc) {

	switch(rc){
		case 1:
			cout << "Connection Refused: unacceptable protocol version\n" << endl;
			break;
		case 2:
			cout << "Connection Refused: identifier rejected\n" << endl;
			break;
		case 3:
			cout << "Connection Refused: broker unavailable\n" << endl;
			break;
		case 4:
			cout << "Connection Refused: bad user name or password\n" << endl;
			break;
		case 5:
			cout << "Connection Refused: not authorised\n" << endl;
			break;
		default:
			cout << "Connection Refused: unknown reason\n" << endl;
			break;
	}
}
