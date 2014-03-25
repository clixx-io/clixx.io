## clixx.io EventFramework

### Rethinking C/C++ for Embedded devices and the Internet-of-Things

The clixx.io EventFramework is a streamlined rethink of C/C++ traditions 
with respect to making small mpu's ever easier to program. It's inspired
by Wiring, and Python "Circuits".

The idea is to move all hardware dependencies to libraries and have
code that will easily run on different processors.

### The easy coding style of the clixx.io Event-Framework

The clixx.io Eventframework is based on having an 'Application'
class which then passes program execution to event-handlers when
particular hardware or software events occur).

	/* Simple Application
	 * 
	 *  A trivial simple example of an clixx.io Application.
	 * 
	*/
	#include <stdio.h>
	#include "clixxIO.hpp"

	class App : public clixxIOApp{

		int loop(){
			// loop handler - This runs repeatedly
			if (getTemperature() > 24)
				aircon.On();
			else
				aircon.Off();
			delay(1000);
		};
		
		int setup(){
			// Setup - register the loop event for continuously running. 
			printf("Program is running");
			
			aircon = clixxIO_Motor('motor1');
			addLoopEvent(&loop());
		};
		
	};

	// -- Main program Section. Simply setup an App class and let it run
	int main(){
	  App m;
	  return m.run();
	} 
	#include "app-callbacks.cpp"	// 

### Setup

The setup() function is executed once at start of the program and
is normally used to initialise devices.

	void setup(){
		// setup - create a timer callback every 2 seconds
		addTimerEvent(2, (void (*)()) &App::timerevent);

        // Setup all of the devices that are needed by the application		
		lcd = clixxIO_i2cCharlcd();
		button1 = clixxIO_Button('button1');
		button2 = clixxIO_Button('button2');
		motor1  = clixxIO_Motor('motor1');
		 
	};

### Loop

The loop() function is an optional method that can be repeatedly
run. It's not necessary to have a program loop() if the program
is running on other events, such as interrupts or serial-data
for example.

    int loop(){
        // loop - this runs repeatedly
        if (getTemperature() > 24)
			setAirconditioning(On);
		else
			setAirconditioning(Off);
		delay(1000);
    };
    
	void setup(){
        addLoopEvent((void (*)()) &App::loop);
    };

### Timers

The vast majority of embedded applications all need timers for some
purpose. Traditionally it's been left to the engineer to interface
to the hardware/operating-system to determine a base for the timing
system of the application. Not only does that require some effort,
it has led to a huge amount of program incompatability as programs
are coded to use the timing hardware of a particular platform rather
than get on with the job of actually doing the timing functions.

The following example shows the same functionality of the above
example only here the checking is done by timer callbacks which
are non-blocking rather than using a blocking delay() call in
the previous example:

    void timerevent(){
        // timerevent - this is run every second
		if (getTemperature() > 24)
			aircon.On();
		else
			aircon.Off();
    };
	void setup(){
		// setup - create a timer to check the temperature every 2 seconds
		addTimerEvent(1, (void (*)()) &App::timerevent);
	};


### Interrupts

From the dawn of programming computers, there haven't been that many nice ways
to handle pin change interrupts from an application. 

Handling pin change interrupts is now moved into the main application object
and is available as part of the core application code rather than moved (usually) a
long way away from the main() function where most of the application logic is
expected to be.

With this structure, handling interrupts becomes part of the main application
functionality.

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

### Serial Port Data

One of the very first things that computers ever did was send data over
serial ports. Unfortunately, until the Wiring language, there's never been
that many consistent interfaces for handling serial data streams.

In the EventFramework, serial data can be received and passed to the
application either in blocks or lines with a callback able to be setup
when the data is available.

	int setup(){
		// Setup - create an event that prints out serial data from a port
		//         as well as a pin change interrupt handler
		addSerialDataEvent(GPIO_PIN1, 9600, (void (*)()) &App::serialData());
	};
   
	int serialData(const char* buffer, const int bufflen){
		//* SerialData - Print out the data received on the port
		printf(buffer)

	}
