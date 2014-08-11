/*
 * ClientSub.cpp
 *
 *  Created on: Jan 11, 2012
 *      Author: rene
 */

#include "clixxIO.hpp"

using namespace std;


#ifdef TARGET_LINUX
ClixxIO_IoTSub::ClixxIO_IoTSub(const char* id) : mosquittopp(id)
{
    // By default, the topic is set to # which is everything
    topic[0] = '#';
    topic[1] = (char ) 0;
}
#endif

ClixxIO_IoTSub::~ClixxIO_IoTSub() 
{
}

int ClixxIO_IoTSub::connect(const char *host, int port, int keepalive, bool clean_session)
{
    return(mosquittopp::connect(host, port, keepalive, clean_session));
    
}

int ClixxIO_IoTSub::disconnect()
{
    return mosquittopp::disconnect();
}

void ClixxIO_IoTSub::on_connect(int rc) {

    C_iotopen( pMainClass );
    
    if (!rc) {
        subscribe(&mid, (const char * ) &topic, 0);
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

        C_iotmessage( pMainClass );

//        cout << message->topic << ": " << message->payload << endl;
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
