
/* Simple Timer

A trivial simple example of using circuits and timers.
*/

#include <stdio.h>
#include "clixxIO.hpp"

class App : public clixxIOApp{

  public:
  
    void timerevent(){
        /* hello Event handler

         Fired once every second.
        */

        printf("Timer Callback\n");
    };
    
    void started(){
        /* started Event handler

        Setup a simple timer to fire every second.
        */
        printf("Application in startup event\n");

        addTimerEvent(5, (void (*)()) &App::timerevent);
    };
    
};

// Final statement should be => App().run()
main(){

  App m;

  // A single fire of m.timerevent() as a test 
  C_timerevent(&m);
  
  m.run();
}

// This method will be hidden somewhere later
void C_timerevent( void* appC) { static_cast<App*>(appC)->timerevent(); }
void C_startedevent( void* appC) { static_cast<App*>(appC)->started(); }
