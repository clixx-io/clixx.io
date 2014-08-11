/* Simple Interrupt counter
 * 
 *  A trivial example showing counting pin interrupts per second.
 *  ie this is a very simple example of an RPS counter
 * 
*/

#include <stdio.h>
#include "clixxIO.hpp"

class App : public clixxIOApp{

    volatile int counter = 0;
    
    int setup(){
        // Setup - create a timer that displays data every second
        //         as well as a pin change interrupt handler
        addTimerEvent(1, (void (*)()) &App::timerevent);
        addPinChangeEvent(GPIO_PIN16, RISING, (void (*)()) &App::PinChangeInterrupt());
    };
    
    int timerevent(){
        // timerevent - fired every second to display the value
        int v = counter; counter = 0;
        if (v == 0)
			printf('.')
		else
            printf("rps = %d\n", v);
    };
    
    int PinChangeInterrupt(){
        /* Interrupt service routine
        Handles a hardware interrupt of some sort
        */
	    ++counter;
	}
};

// Traditional main with Application object

main(){
  App m;
  m.run();
}
