
/* Simple Timer

A trivial simple example of using circuits and timers.
*/

// #include "clixxIO_EventQ.hpp"

#include <stdio.h>
#include "clixxIO.hpp"

class App : public clixxIOApp{

    public:
    
    int timerevent(){
        /* hello Event handler

         Fired once every second.

        */

        printf("Timer Event");
    };
    
    int started(){
        /* started Event handler

        Setup a simple timer to fire every second.
        */

        // Timer(seconds, event, persist=False)
        // Timer(1, Event.create("timerevent"),persist=True).register(self)
    };
    
};

// App().run()

main(){
  App m;
  m.run();
}

