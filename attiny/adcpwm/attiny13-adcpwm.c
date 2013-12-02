#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000UL
#include <util/delay.h>

int main (void)
{
   DDRB |= (1 << 1); // my change here - use pb1
   DDRB |= (1 << 0); // my change here - use pb0

   ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz

   ADMUX |= (0 << REFS0); // Set ADC ref to INTERNAL 5v
   ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading

   ADMUX |= (1 << MUX1); //use ADC2 on PB4

   ADCSRA |= (1 << ADATE);  // My change.. Tiny free-Running Mode enabled.
   ADCSRA |= (1 << ADEN);  // Enable ADC

   ADCSRA |= (1 << ADIE);  // Enable ADC Interrupt
   sei();   // Enable Global Interrupts

   ADCSRA |= (1 << ADSC);  // Start A2D Conversions

   for(;;)  // Loop Forever
   {
   }
}

ISR(ADC_vect)
{
   if(ADCH < 128)
   {
      PORTB |= (1 << 1); // my change here
      PORTB &= ~(1 << 0); // Turn off LED2
   }
     
   else
   {
      PORTB &= ~(1 << 1); // my change here
      PORTB |= (1 << 0); // Turn on LED2
   }
}
