/*
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
 */
#include "clixxIO.hpp"
#include "iohelp.h"
#include <avr/io.h>

/* -------------------------------------------------------------------------
 *
 * GPIO/Pin Functions. Inspired by Wiring
 *
 * ------------------------------------------------------------------------*/


// Definitions of all Input and Output Pins according to their connector
int input_pins[] = {D1_O,D2_O};
int output_pins[] = {D1_I,D2_I};

int getInputPin(int portname){ 
    return input_pins[portname]; 
};

int getOutputPin(int portname){ 
    return output_pins[portname]; 
};

void pinMode(int pin, int value){
    
    if (value == OUTPUT){
        DDRB |= (1 << pin);    	    // pin is an output
    } else {
        DDRB &= ~(1<< pin);    		// Turn pin off
    }
}

void digitalWrite(int pin, int value){
    
	if (value){
		PORTB |= (1 << pin);   		// Turn pin on
    } else {
        PORTB &= ~(1 << pin);    	// else Turn pin off
    }
}

int digitalRead(int pin){

    return (PINB & (1 << pin));
}

void analogWrite(int pin, int value){
}

int analogRead(int pin){
    return(0);
}

/** Writes a string to the serial port
*
* This function writes a string to the serial port
* 
*/
int clixxIO_Button::pressed(){	  

	return adcRead(ADC0, 1, 3);
    	
}

/** Writes a string to the serial port
*
* This function writes a string to the serial port
* 
*/
int clixxIO_Switch::On(){
    digitalWrite(_gpiopin, HIGH);

	PORTB |= (1<<_gpiopin);    		// Else turn pin on

    return(0);
}

/** Writes a string to the serial port
*
* This function writes a string to the serial port
* 
*/
int clixxIO_Switch::Off(){
    digitalWrite(_gpiopin, LOW);
//	PORTB &= ~(1<<D1_O);    		// Turn pin off
    return(0);
}	  
