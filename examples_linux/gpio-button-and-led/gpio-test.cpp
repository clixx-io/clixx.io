 /* ===============================================================================
 * 
 * Copyright (c) 2014 clixx.io
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the Clixx.io nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CLIXX.IO BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 =================================================================================
 */

#include "clixxIO_GPIO.h"
 
using namespace std;
 
int main (void)
{
 
    string inputstate;
    int i = 0;
    GPIOPin* button1 = new GPIOPin("24"); //create new GPIO object to be attached to  GPIO17
    GPIOPin* led1 = new GPIOPin("22"); //create new GPIO object to be attached to  GPIO4
 
    cout << " GPIO pins exported" << endl;
 
    button1->setdir_gpio("in"); //GPIO4 set to output
    led1->setdir_gpio("out"); // GPIO17 set to input
 
    cout << " Set GPIO pin directions" << endl;

    led1->setval_gpio("1"); // turn LED ON
    usleep(500000);  // wait for 0.5 seconds
    led1->setval_gpio("0"); // turn LED ON
 
    while(i < 20)
    {
        usleep(500000);  // wait for 0.5 seconds
        button1->getval_gpio(inputstate); //read state of GPIO17 input pin
        cout << "Current input pin state is " << inputstate  <<endl;
        if(inputstate == "0") // if input pin is at state "0" i.e. button pressed
        {
            cout << "input pin state is \"Pressed \".n Will check input pin state again in 20ms "<<endl;
            usleep(20000);
            cout << "Checking again ....." << endl;
            button1->getval_gpio(inputstate); // checking again to ensure that state "0" is due to button press and not noise

            if(inputstate == "0")
            {
                cout << "input pin state is definitely \"Pressed\". Turning LED ON" <<endl;
                led1->setval_gpio("1"); // turn LED ON
 
                cout << " Waiting until pin is unpressed....." << endl;
                while (inputstate == "0"){
                button1->getval_gpio(inputstate);
                };
                cout << "pin is unpressed" << endl;
 
            }
            else
                cout << "input pin state is definitely \"UnPressed\". That was just noise." <<endl;
 
        }
        led1->setval_gpio("0");
        i++;
    }

    cout << "Releasing heap memory and exiting....." << endl;
    delete button1;
    delete led1;

    return 0;
}
