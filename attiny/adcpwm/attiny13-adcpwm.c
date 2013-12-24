#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 1000000UL
#include <util/delay.h>

#define D1_I PB2
#define D1_O PB1
#define D1_S PB5

#define D2_I PB3
#define D2_O PB4
#define D2_S PB0

#define LED_PIN D2_O

int main (void)
{
   DDRB |= (1 << 1); // my change here - use pb1
   DDRB |= (1 << 0); // my change here - use pb0

   ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MHz

   ADMUX |= (0 << REFS0); // Set ADC ref to INTERNAL 5v
   ADMUX |= (1 << ADLAR); // Left adjust ADC result to allow easy 8 bit reading

   // ADMUX |= (1 << MUX1); //use ADC2 on PB4
   ADMUX |= (1 << MUX1)|(1 << MUX0); //use ADC2 on PB3

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
      PORTB |=  _BV( LED_PIN );
   }
     
   else
   {
  	  PORTB &=~ _BV( LED_PIN );
   }
}
