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
#include <avr/io.h>
#include <util/delay.h>
#include "clixxIO.hpp"
#include "iohelp.h"

/* -------------------------------------------------------------------------
 *
 * GPIO/Pin Functions. Inspired by Wiring
 *
 * ------------------------------------------------------------------------*/

/** Turns a switch off
*
* This function turns the switch off
* 
*/
void pinMode(int pin, int value){
    
    if (value == OUTPUT){
        DDRB |= (1 << pin);    	    // pin is an output
    } else {
        DDRB &= ~(1<< pin);    		// Turn pin off
    }
}

/** Turns a switch off
*
* This function turns the switch off
* 
*/
void digitalWrite(int pin, int value){
    
    if (value){
        PORTB |= (1 << pin);   		// Turn pin on
    } else {
        PORTB &= ~(1 << pin);    	// else Turn pin off
    }
}

/** Turns a switch off
*
* This function turns the switch off
* 
*/
int digitalRead(int pin){

    return (PINB & (1 << pin));
}

/** Turns a switch off
*
* This function turns the switch off
* 
*/
int analogRead(int pin){
    return(0);
}

/** Loads a pin
*
* This function looks up the configuration for a pin from
* EEPROM.
* 
*/
clixxIOGPIOPin::clixxIOGPIOPin()
{
    
}

clixxIOGPIOPin::~clixxIOGPIOPin()
{
    
}

void clixxIOGPIOPin::digitalWrite(short val)
{
    if (val != 0){
        PORTB |= (1 << _gpionum);   // Turn pin on
    } else {
        PORTB &= ~(1 << _gpionum);  // else Turn pin off
    }
}

int clixxIOGPIOPin::digitalRead()
{
    return (PINB & (1 << _gpionum));
}

int clixxIOGPIOPin::configure(short pinnumber,short direction, bool source)
{
    _gpionum = pinnumber;
    _direction = direction;
    
    if (direction == OUTPUT){
        DDRB |= (1 << pinnumber);    	    // pin is an Output
    } else {
        DDRB &= ~(1<< pinnumber);    		// Pin as an Input
    }
    if (source)
    {
      #if defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__)  \
       || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644PA__) \
       || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328PA__) \
       || defined (__AVR_ATmega164P__) || defined (__AVR_ATmega164A__)
          // PORTD |= (1 << pinnumber);          // PD0 is now an output and is sourcing VCC
      #endif
    } else
    {
      #if defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__)  \
       || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644PA__) \
       || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328PA__) \
       || defined (__AVR_ATmega164P__) || defined (__AVR_ATmega164A__)
        // PORTD &= ~(1 << pinnumber);            // PDO is still an output and is now sinking 0V
      #endif
    }

    return(0);
}

/** Writes a string to the serial port
*
* This function writes a string to the serial port
* 
*/
bool clixxIO_Button::pressed(){	  

    return (PINB & (1 << _gpionum));

}

/** Turns a switch On
*
* This function turns the switch On
* 
*/
void clixxIO_Switch::On(){
    
    PORTB |= (1<<_gpionum);    		// Turn turn pin on

}

/** Turns a switch off
*
* This function turns the switch off
* 
*/
void clixxIO_Switch::Off(){
    
    PORTB &= ~(1<<D1_O);    		// Turn pin off

}
/** analogWrite
*
* Performs a PWM function on a pin
* 
*/
void analogWrite(int pin, int value){
}
