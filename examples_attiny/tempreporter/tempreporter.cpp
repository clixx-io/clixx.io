/* Simple Timer

A simple example of using timers.

*/

#define F_CPU 8000000UL

#include <stdio.h>
#include "clixxIO.hpp"
#include "tempreporter-config.hpp"

#include <avr/interrupt.h>
#include <util/delay.h>
#include "core_AVR/iohelp.h"

#define TEMP_OFFSET 17

class App : public clixxIOApp{

  public:
  
    void setup(){
        
		Serial.begin();
		sei();
		
		Serial.puts("Hello - I am alive\n\r");

		adcInit(ADC4);

		DDRB |= (1<<D1_O);    			// digital-1 out an output with LED
    };
    
    void loop(){
		
		static char str[25];

		// LED On
		PORTB |= (1<<D1_O);				// Turn pin on
		_delay_ms(200);					// Delay 200 millisecond
		
		// Process Temperatures via ADC
		int temp = adcRead(ADC4,1,3);
		
		snprintf(str, sizeof(str), "Temp=%d %d\n\r", temp,  temp - 273 + TEMP_OFFSET);
		Serial.puts(str);

		// LED Off
		PORTB &= ~(1<<D1_O);			// Turn pin off
		_delay_ms(200);					// Delay 200 millisecond

    };
    
};


// Main program Section. Simply setup an App class and let it run
int main(){
 
    App m;

    return m.run();

}

// Autogenerated interface between the event interface and application class
#include "tempreporter-callbacks.cpp"
