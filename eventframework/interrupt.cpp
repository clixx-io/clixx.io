
/* Simple Loop

A trivial simple example of using circuits and timers.
*/

#include <stdio.h>
#include "clixxIO.hpp"

class App : public clixxIOApp{

    volatile int counter = 0;
    
    int loopevent(){
        /* hello loop handler

         This runs repeatedly
        */
        
        if (counter == 999)
            printf("Counter reached 999");
    };
    
    int started(){
        /* started Event handler
        Setup a loop event to continuously run. This would
        be like the for(;;) loop in a traditional embedded
        program
        */

        addLoopEvent(&loopevent());
        addPinChangeEvent(GPIO_PIN16, RISING, &PinChangeInterrupt());
    };
    
    int PinChangeInterrupt(){
        /* Interrupt service routine
        Handles a hardware interrupt of some sort
        */
	    if (++counter == 1000)
	       counter = 0;
	}
};

// Traditional main with Application object

main(){
  App m;
  m.run();
}
