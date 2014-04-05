/*--------------------------------------------------------------------------*
* Hardware definitions for Microboot
*---------------------------------------------------------------------------*
* 01-Apr-2014 ShaneG
*
*  Initial version.
*--------------------------------------------------------------------------*/
#ifndef __HARDWARE_H
#define __HARDWARE_H

// Baud rate to use
#define BAUD_RATE 57600

// Port and pin number to use (ATtiny only)
#define UART_PORT PORTB
#define UART_DDR  DDRB
#define UART_PIN  PINB2

#endif /* __HARDWARE_H */
