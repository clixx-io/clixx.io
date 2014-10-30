/*--------------------------------------------------------------------------*
* IO convenience functions
*---------------------------------------------------------------------------*
* 16-Apr-2014 ShaneG
*
* These functions provide simple ways to deal with common IO functions.
*--------------------------------------------------------------------------*/
#ifndef __IOHELP_H
#define __IOHELP_H

//--- Required definitions
#include <stdint.h>
#include <stdbool.h>
#include <avr/io.h>

// Definitions of all Input and Output Pins according to their connector
#ifdef TARGET_AVR

  // These Pin definitions for AVR processors
  #if defined (__AVR_ATtiny25__) || defined (__AVR_ATtiny45__) || defined (__AVR_ATtiny85__)
    // Pin Definitions for the Attiny85
    #define D1_I PB3
    #define D1_O PB4
    #define D1_S PB0

    #define D2_I PB2
    #define D2_O PB1
    #define D2_S PB5

    #define PIN_A1i PB4
    #define PIN_A1o PB2

    // int input_pins[] = {D1_O,D2_O};
    // int output_pins[] = {D1_I,D2_I};
    
  #elif defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__)  \
   || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644PA__) \
   || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328PA__) \
   || defined (__AVR_ATmega164P__) || defined (__AVR_ATmega164A__)
   
  #endif
  
#endif

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
// Digital helper functions
//---------------------------------------------------------------------------

/** Macro to set a pin high
*
* Set the output value of a pin on port B high
*
* @param pin the pin number to change
*/
#define pinHigh(pin) \
PORTB |= (1 << pin)

/** Set a pin low
*
* Set the output value of a pin on port B low
*
* @param pin the pin number to change
*/
#define pinLow(pin) \
PORTB &= ~(1 << pin)

/** Set the state of a pin
*
* Alternative function to pinHigh/pinLow macros.
*
* @param pin the pin number to change
* @param value the value to set (true = high, false = low)
*/
void pinSet(uint8_t pin, bool value);

/** Get the state of a pin
*
* @param pin the pin number to read
*/
#define pinGet(pin) \
(PINB & (1 << pin))

//---------------------------------------------------------------------------
// Analog helper functions
//---------------------------------------------------------------------------

/** Analog pin inputs
*/
typedef enum _ANALOG {
  ADC0 = 0x00, //!< ADC0, PB5
  ADC1, //!< ADC1, PB2
  ADC2, //!< ADC2, PB4
  ADC3, //!< ADC3, PB3
  ADC4 = 0x0F //!< ADC4 - internal temperature sensor
  } ANALOG;

/** Initialise the specified analog pin
*
* This function initialises the ADC accessory as well as setting up the input
* pin attached to the ADC. The set up uses VCC as the reference voltage. If
* you need to change this do so after initialising all the ADC inputs you
* want to use.
*
* @param adc the ADC input to initialise.
*/
void adcInit(ANALOG adc);

/** Read a value from the analog input
*
* Read a 10 bit value from the specified input with option input skipping and
* averaging.
*
* @param adc the ADC input to read
* @param skip the number of samples to skip (read and ignore).
* @param average the number of samples to average (must be >= 1)
*
* @return the sample value.
*/
uint16_t adcRead(ANALOG adc);

//---------------------------------------------------------------------------
// PWM helpers
//---------------------------------------------------------------------------

/** Initialise a PWM pin
*
* Set up a PWM output pin.
*
* @param pin the pin number to initialise.
* @param frequency the frequency to operate at.
*/
void pwmInit(uint8_t pin, uint16_t frequency);

/** Set the value of a PWM pin
*
* Set the output for a PWM pin.
*
* @param pin the pin number of the output to modify.
* @param value the value for the duty cycle ranging from 0 (fully off) to 255
* (fully on).
*/
void pwmOut(uint8_t pin, uint8_t value);

//---------------------------------------------------------------------------
// Software SPI
//---------------------------------------------------------------------------

/** Initialise pins for software SPI
*
* It is not necessary to call this in order to use software SPI, it is simply
* a convenience function to do the necessary setup. You can initialise the
* pins directly if needed.
*
* @param sck the pin to use for the SCK output
* @param mosi the pin to use for the MOSI output
* @param miso the pin to use for the MISO input
*/
void sspiInit(uint8_t sck, uint8_t mosi, uint8_t miso);

/** Transfer data to a slave (MSB first)
*
* @param sck the pin to use for the SCK output
* @param mosi the pin to use for the MOSI output
* @param data the data to transfer
* @param bits the number of bits to transfer
*/
void sspiOutMSB(uint8_t sck, uint8_t mosi, uint16_t data, uint8_t bits);

/** Transfer data from a slave (MSB first)
*
* @param sck the pin to use for the SCK output
* @param miso the pin to use for the MISO input
* @param bits the number of bits to transfer
*
* @return a 16 bit value containing the data in the lowest bits.
*/
uint16_t sspiInMSB(uint8_t sck, uint8_t miso, uint8_t bits);

/** Transfer data to and from a slave (MSB first)
*
* @param sck the pin to use for the SCK output
* @param mosi the pin to use for the MOSI output
* @param miso the pin to use for the MISO input
* @param data the data to transfer out
* @param bits the number of bits to transfer
*
* @return a 16 bit value containing the read data in the lowest bits.
*/
uint16_t sspiInOutMSB(uint8_t sck, uint8_t mosi, uint8_t miso, uint16_t data, uint8_t bits);

/** Transfer data to a slave (LSB first)
*
* @param sck the pin to use for the SCK output
* @param mosi the pin to use for the MOSI output
* @param data the data to transfer
* @param bits the number of bits to transfer
*/
void sspiOutLSB(uint8_t sck, uint8_t mosi, uint16_t data, uint8_t bits);

/** Transfer data from a slave (LSB first)
*
* @param sck the pin to use for the SCK output
* @param miso the pin to use for the MISO input
* @param bits the number of bits to transfer
*
* @return a 16 bit value containing the data in the lowest bits.
*/
uint16_t sspiInLSB(uint8_t sck, uint8_t miso, uint8_t bits);

/** Transfer data to and from a slave (LSB first)
*
* @param sck the pin to use for the SCK output
* @param mosi the pin to use for the MOSI output
* @param miso the pin to use for the MISO input
* @param data the data to transfer out
* @param bits the number of bits to transfer
*
* @return a 16 bit value containing the read data in the lowest bits.
*/
uint16_t sspiInOutLSB(uint8_t sck, uint8_t mosi, uint8_t miso, uint16_t data, uint8_t bits);

#ifdef __cplusplus
}
#endif

#endif /* __IOHELP_H */
