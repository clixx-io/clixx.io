
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

        addTimerEvent(1, (void (*)()) &App::timerevent);
    };
    
};

// Final statement should be => App().run()
main(){

  App m;

  m.run();

}

#include "timer-App.cpp"
