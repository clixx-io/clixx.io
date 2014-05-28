
# Based on the post at https://sites.google.com/site/qeewiki/books/avr-guide/external-interrupts-on-the-atmega328

#include <avr/io.h>
#include <stdint.h>            // has to be added to use uint8_t
#include <avr/interrupt.h>    // Needed to use interrupts
volatile uint8_t portbhistory = 0xFF;     // default is high because the pull-up

int main(void)
{
    DDRB &= ~((1 << DDB0) | (1 << DDB1) | (1 << DDB2)); // Clear the PB0, PB1, PB2 pin
    // PB0,PB1,PB2 (PCINT0, PCINT1, PCINT2 pin) are now inputs

    PORTB |= ((1 << PORTB0) | (1 << PORTB1) | (1 << PORTB2)); // turn On the Pull-up
    // PB0, PB1 and PB2 are now inputs with pull-up enabled

    PCICR |= (1 << PCIE0);     // set PCIE0 to enable PCMSK0 scan
    PCMSK0 |= (1 << PCINT0);   // set PCINT0 to trigger an interrupt on state change 

    sei();                     // turn on interrupts

    while(1)
    {
    /*main program loop here */
    }
}

ISR (PCINT0_vect)
{
    uint8_t changedbits;

    changedbits = PINB ^ portbhistory;
    portbhistory = PINB;

    if(changedbits & (1 << PB0))
    {
    /* PCINT0 changed */
    }

    if(changedbits & (1 << PB1))
    {
    /* PCINT1 changed */
    }

    if(changedbits & (1 << PB2))
    {
    /* PCINT2 changed */
    }
}
