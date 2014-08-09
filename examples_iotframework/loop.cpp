/* Simple Loop
 * 
 *  A trivial simple example of a clixx.io loop.
 * 
*/

#include <stdio.h>
#include "clixxIO.hpp"

class App : public clixxIOApp{

    int loop(){
        /* loop handler
         * 
         * This runs repeatedly
         * 
        */
        printf("Program is running");
    };
    
    int setup(){
        /* started Event handler
        Setup and register the loop event to continuously run. 
        */
        addLoopEvent(&loop());
    };
    
};

// Traditional main with Application object
main(){
  App m;
  m.run();
}
