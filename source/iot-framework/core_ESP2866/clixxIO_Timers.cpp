
// File to hold timer interrupt setup
#include <avr/io.h>
#include <avr/interrupt.h>


#include <avr/io.h>


#if defined (__AVR_ATmega324P__) || defined (__AVR_ATmega324A__)  \
   || defined (__AVR_ATmega644P__) || defined (__AVR_ATmega644PA__) \
   || defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328PA__) \
   || defined (__AVR_ATmega164P__) || defined (__AVR_ATmega164A__)

void InitPort(void)
{
//Init PB1/OC1A and PB2/OC1B pins as output
DDRB|=(1<<PB1)|(1<<PB2);
}
void InitTimer1(void)
{
//Set Initial Timer value
TCNT1=0;
//set non inverted PWM on OC1A pin
//and inverted on OC1B
TCCR1A|=(1<<COM1A1)|(1<<COM1B1)|(1<<COM1B0);
//set top value to ICR1
ICR1=0x00FF;
//set corrcet phase and frequency PWM mode
TCCR1B|=(1<<WGM13);
//set compare values
OCR1A=0x0064;
OCR1B=0x0096;
}
void StartTimer1(void)
{
//Start timer with prescaller 64
TCCR1B|=(1<<CS11)|(1<<CS10);
}
int z(void)
{
InitPort();
InitTimer1();
StartTimer1();
	while(1)
	{
		//do nothing
	}
}

#endif
