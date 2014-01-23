
/* Simple Timer

A trivial simple example of using circuits and timers.
*/

#include <stdio.h>
#include "clixxIO.hpp"

class App : public clixxIOApp{

    void timerevent(){
        /* hello Event handler

         Fired once every second.
        */

        printf("Timer Event");
    };
    
    int started(){
        /* started Event handler

        Setup a simple timer to fire every second.
        */

		void *(dp)() = dynamic_cast<void (class App::*)()>(&timerevent);
		
		// if (dp != null)
		//	dp->methodInDerivedClass();
        
        // addTimerEvent(5, &timerevent);
    };
    
};

// Final statement should be => App().run()

main(){
  App m;
  m.run();
}

