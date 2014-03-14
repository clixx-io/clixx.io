/* Simple Loop

A trivial simple example of using circuits and timers.
*/

#include <stdio.h>
#include "clixxIO.hpp"

class App : public clixxIOApp{

    int loopevent(){
        /* hello loop handler

         This runs repeatedly
        */

        printf("Program is running");
    };
    
    int started(){
        /* started Event handler

        Setup a loop event to continuously run. This would
        be like the for(;;) loop in a traditional embedded
        program
        
        */

        addLoopEvent(&loopevent());
    };
    
};

// Traditional main that is called by linker
main(){
  App m;
  m.run();
}
