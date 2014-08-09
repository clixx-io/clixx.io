/*
 * Copyright (c) 2014 clixx.io
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the Clixx.io nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CLIXX.IO BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#include "clixxIO.hpp"

ClixxIO_IoTSub::ClixxIO_IoTSub(const char* id){

}

ClixxIO_IoTSub::~ClixxIO_IoTSub() {

}

int ClixxIO_IoTSub::connect(){

	return(0);
}

int ClixxIO_IoTSub::subscribe(const char* topic){

	return(0);
}

int ClixxIO_IoTSub::disconnect(){

	return(0);
}

/*
void ClixxIO_IoTSub::on_connect(int rc) {

	if (!rc) {
		subscribe(&mid, "#", 0);
	}
	else {
		print_error_connection(rc);
	}
}

void ClixxIO_IoTSub::on_subscribe(uint16_t mid, int qos_count, const uint8_t *granted_qos) {

	cout << "Subscribed (mid: " << mid << "): " << granted_qos[0] << endl;
	for(int i=1; i<qos_count; i++){
		cout << ", " <<granted_qos[i] << endl;
	}
}

void ClixxIO_IoTSub::on_message(const struct mosquitto_message *message) {

	if(message->payloadlen){	// message contains a payload
			cout << message->topic << ": " << message->payload << endl;
	} else {
		cout << message->topic << ": (null)" << endl;
	}
}

void ClixxIO_IoTSub::print_error_connection(int rc) {

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
*/
