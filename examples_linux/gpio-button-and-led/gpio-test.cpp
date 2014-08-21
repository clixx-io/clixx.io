#include "clixxIO_GPIO.h"
 
using namespace std;
 
int main (void)
{
 
    string inputstate;
    int i = 0;
    GPIOClass* button1 = new GPIOClass("24"); //create new GPIO object to be attached to  GPIO17
    GPIOClass* led1 = new GPIOClass("22"); //create new GPIO object to be attached to  GPIO4
 
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
        button1->setval_gpio("0");
        i++;
    }

    cout << "Releasing heap memory and exiting....." << endl;
    delete button1;
    delete led1;

    return 0;
}
