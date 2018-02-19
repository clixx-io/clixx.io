/*
 * Main.cpp
 *
 *  Created on: Jan 7, 2012
 *      Author: rene
 */

#include "clixxIO.hpp"
#include <mosquittopp.h>
#include <iostream>

#include <stdio.h>

using namespace std;

class App : public clixxIOApp{

  private:
  
    mosquittopp::mosquittopp *m;

  public:

    void setup(){
        /* setup Event handler

         This is a built in handler that will get called on startup
         and provides for initialisation requirements.

        */
        puts("Application in Startup event");

        m = new mosquittopp::mosquittopp("hello");

        m->lib_init();

        puts("about to connect");

        m->connect("test.mosquitto.org", 1883, 60, true);

    };

    void iotmessage(){
        puts("mqtt message received");
    };
    
    void iotopen(){
        puts("mqtt channel opened");
    };
    
    void iotclosed(){
        puts("mqtt Channel closed");
    };

    void loop(){
        int rc;
        do {
            rc = m->loop(-1);
        } while(rc == MOSQ_ERR_SUCCESS);
    };

    void shutdown(){
        /* shutdown Event handler

         This is a built in handler is run before the program is
         shut down. It allows for files to be closed and/or
         resources to be released.

        */
        puts("Application in Shutdown event");
        m->disconnect();
        m->lib_cleanup();

    };

};

// Main program Section. Simply setup an App class and let it run
int main(){
 
  App *a;
  
  a =  new App();
  
  return a->run();

}

void C_setupevent(     void* appC) { static_cast<App*>(appC)->setup(); }
void C_shutdownevent(  void* appC) { static_cast<App*>(appC)->shutdown(); }
void C_loopevent(      void* appC) { static_cast<App*>(appC)->loop();  }
void C_pinchange(      void* appC) {  }
void C_timerevent(     void* appC) {  } 

void C_serialchar(     void* appC) {  }
void C_serialline(     void* appC) {  }
void C_serialopen(     void* appC) {  }
void C_serialclose(    void* appC) {  }

void C_iotmessage(     void* appC) { static_cast<App*>(appC)->iotmessage();  }
void C_iotopen(        void* appC) { static_cast<App*>(appC)->iotopen();   }
void C_iotclose(       void* appC) { static_cast<App*>(appC)->iotclosed();  }
