/* Program

   A simple program created by the clixx.io generator now ready for 
   customisation.

*/

#include <stdio.h>
#include <string.h>
#include "clixxIO.hpp"
#include "remoteswitch-config.hpp"

clixxIO_Switch Switch1(SWITCH1_CONNECTOR);

class App : public clixxIOApp{

  public:

    void setup(){
        /* setup Event handler

         This is a built in handler that will get called on startup
         and provides for initialisation requirements.

        */
        Serial.begin();

        // Some Serial fiddling
        Serial.echo = '1'; Serial.linemode = '1'; Serial.iotmode = 1;

        Debug.puts("\n\rApplication in setup event\n\r");

        addIoTSubEvent("/remote/switch",(void (*)()) &App::iotmessage);
    };

    void iotmessage(){
        /* IoT Message handler
 
         This gets called when an IoT sub is received.

        */
        Debug.puts("IoT Message Callback\n\rPacket=");
        Debug.puts(Serial.iotpacket());

        if (strncmp(Serial.iotpacket(),"On",BUFFSIZE_LINELEN) == 0){

            Switch1.On();
            Debug.puts("\r\nSwitch was turned On\n\r");

        } else {

            Switch1.Off();
            Debug.puts("\r\nSwitch was turned Off\n\r");

        }
    };

};

// Main program Section. Simply setup an App class and let it run
int main(){

  App m;

  return m.run();

}

// Autogenerated interface between the event interface and application class
#include "remoteswitch-callbacks.cpp"
