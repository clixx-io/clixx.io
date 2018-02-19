GPIO Example
------------

This is a code example showing reading and writing to the GPIO in C++
using the basic GPIOPin class.

Lets' have a look at the code. First we set up a new object for each
gpio pin that we wish to access:

    GPIOPin* button1 = new GPIOPin("24"); //create new GPIO object to be attached to  GPIO17
    GPIOPin* led1 = new GPIOPin("22"); //create new GPIO object to be attached to  GPIO4

Once the variables are initialised, we then need to set the directions of the internal
transistors so that they either work to direct logic from outside the computer to the
inside, or from the inside to the out. This is called the Logic Direction.

    button1->setdir("in"); //GPIO4 set to output
    led1->setdir("out"); // GPIO17 set to input

Once this is done, we can now read and write to the pins as required.

To flash the LED, this is done with this code:

    led1->setval("1"); // turn LED ON
    usleep(500000);  // wait for 0.5 seconds
    led1->setval("0"); // turn LED ON

To read from the button, we read its value:

    button1->getval(inputstate); //read state of GPIO17 input pin

Finally, when everything is done, you need to delete the object which disconnects
internally from the GPIO:

    cout << "Releasing heap memory and exiting....." << endl;
    delete button1;
    delete led1;

