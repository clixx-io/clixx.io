/*--------------------------------------------------------------------------*
* Analog input helpers
*---------------------------------------------------------------------------*
* 15-Apr-2014 ShaneG
*
* These functions provide simple and accurate analog value reading.
*--------------------------------------------------------------------------*/
#include <avr/io.h>
#include "iohelp.h"

/** Initialise the specified analog pin
*
* This function initialises the ADC accessory as well as setting up the input
* pin attached to the ADC. The set up uses VCC as the reference voltage. If
* you need to change this do so after initialising all the ADC inputs you
* want to use.
*
* @param adc the ADC input to initialise.
*/
void adcInit(ANALOG adc) {
  // Make sure the ADC convertor is setup
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  // Switch off digital for the appropriate pin
  uint8_t mask, didr;
  if(adc==ADC0) {
    mask = 1 << PINB5;
    didr = 1 << ADC0D;
    }
  else if(adc==ADC1) {
    mask = 1 << PINB2;
    didr = 1 << ADC1D;
    }
  else if(adc==ADC2) {
    mask = 1 << PINB4;
    didr = 1 << ADC2D;
    }
  else if(adc==ADC3) {
    mask = 1 << PINB5;
    didr = 1 << ADC3D;
    }
  else // ADC4 or invalid pin, just ignore it
    return;
  // Make sure the pin is an input and pull up is disabled
  PORTB &= ~mask;
  DDRB &= ~mask;
  // Disable digital input for the pin
  DIDR0 |= didr;
  }

/** Read a value from the analog input
*
* Read a 10 bit value from the specified input with option input skipping and
* averaging.
*
* @param adc the ADC input to read
*
* @return the sample value.
*/
uint16_t adcRead(ANALOG adc) {

  // Change the reference voltage and select input
  uint8_t muxval = adc;
  if(adc==ADC4) // Need 1.1V reference
    muxval = 0x80 | adc;
  ADMUX = muxval;
  uint16_t value;
  // Start the conversion, then wait for it to finish
  ADCSRA |= (1 << ADSC);
  while(ADCSRA & (1 << ADSC));
  // Read the current value and add it to the running total
  value = ADCL;
  return( value | (ADCH << 8));
}

