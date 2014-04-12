/* 
	AVR Software-Uart Demo-Application 
	Version 0.4, 10/2010
	
	by Martin Thomas, Kaiserslautern, Germany
	<eversmith@heizung-thomas.de>
	http://www.siwawi.arubi.uni-kl.de/avr_projects
	
	modified 2014: david.lyon at clixx.io for attiny85
*/

/* 
Test environment/settings: 
- avr-gcc 4.3.3/avr-libc 1.6.7 (WinAVR 3/2010)
- Atmel ATmega324P @ 8MHz internal RC, ATtiny85 @ 1MHz internal RC
- 2400bps
*/

#define WITH_STDIO_DEMO   0 /* 1: enable, 0: disable */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "softuart.h"

#if WITH_STDIO_DEMO
#include <stdio.h>

// interface between avr-libc stdio and software-UART
static int my_stdio_putchar( char c, FILE *stream
{
	if ( c == '\n' ) {
		softuart_putchar( '\r' );
	}
	softuart_putchar( c );

	return 0;
}

FILE suart_stream = FDEV_SETUP_STREAM( my_stdio_putchar, NULL, _FDEV_SETUP_WRITE );

static void stdio_demo_func( void ) 
{
	stdout = &suart_stream;
	printf( "This output done with printf\n" );
	printf_P( PSTR("This output done with printf_P\n") );
}
#endif /* WITH_STDIO_DEMO */

static void setup()
{
	 DDRB |= (1<<PB4);    	///PB5/digital 13 is an output
}

static void processcommand(const char *buffer)
{
	int e = 0;
	int pin = 0;
	int port = 0;
	int v = 0;
	
	switch (*buffer)
	{
		case '?' : 	softuart_puts_P( "\r\nQuestionmark command\r\n" );
					if (*(buffer+1) != 'd')
					{
						e = 4;
						break;
					}
					port = *(buffer+2) - 48;		// Port Number ie 1,2,3 etc
					softuart_puts_P( "Checking digital Port d" );
					softuart_putchar( port + 48);

					if (*(buffer+3) == 'i')			// Pin Number ie i,o
					{
						pin = 1;
						softuart_puts_P( "-input" );
					} else if (*(buffer+3) == 'o')
					{
						pin = 0;
						softuart_puts_P( "-output" );
					} else
					{
						e = 5;
						break;
					}
					
					softuart_puts_P( "\r\nValue is " );
					softuart_putchar( '1');
					break;
					
		case 'd' : 	softuart_puts_P( "\r\nDigital command\r\n" );
					port = *(buffer+1) - 48;		// Port Number ie 1,2,3 etc
					pin  = *(buffer+2) - 48;		// Pin  Number ie i,o,* etc
					if (*(buffer+3) != '=')
					{
						e = 2;
						break;
					}
					
					if (*(buffer+4) == '0')			// Value, 0=Off, 1=On
					{
						v = 0;
						PORTB &= ~(1<<PB4);    		// Turn pin off
						
					} else if (*(buffer+4) == '1')
					{
						v = 1;
						PORTB |= (1<<PB4);    		// Else turn pin on
						
					} else
					{
						e = 3;
						break;
					}
					softuart_puts_P( "\r\nPin d");
					softuart_putchar( port + 48);
					softuart_putchar( pin  + 48);
					softuart_puts_P( " set to ");
					softuart_putchar( v + 48);
					
					break;

		case 'a' : 	softuart_puts_P( "\r\nAnalog command\r\n" );
					break;
					
		case 'h' : 	softuart_puts_P( "\r\nHelp\r\n"
		                             " Commands are fixed format\r\n"
		                             " Set Pin = d<port><pin>=<1/0>\r\n"
		                             "           eg \"d1o=1\" to set Digital Port-1 Output to On\r\n"
		                             "           eg \"d1o=0\" to set Digital Port-1 Output to Off\r\n"
		                             " Read Pin = ?<port><pin>\r\n"
		                             "           eg \"?d1i\" to print value of Digital Port-1 Input\r\n"
		                             );
					break;

		default  : e = 1;
	}
	
	if (e)
	{
		softuart_putchar( '\n' );
		softuart_putchar( '[' );
		softuart_putchar( *buffer );
		softuart_putchar( ']' );
		softuart_puts_P( "\r\n" );    // "implicit" PSTR
		softuart_puts_P( "!Bad command\r\n$ " );    // "implicit" PSTR
	} else
		softuart_puts_P( "\r\n$ " );    // "implicit" PSTR
 
}

void buildbuffer(unsigned char c)
{

	#define bufflen 10
	static char buffer[bufflen];
	static unsigned char buffpos = 0;

	if (c == '\r')
	{
		processcommand(&buffer);
		buffpos = 0;
		
	} 	else
	{
		if (buffpos < bufflen)
		{
			buffer[buffpos++] = c;
		}
	}
	
}


int main(void)
{
	char c;
	static const char pstring[] PROGMEM = 
		"adapted for Atmel AVR by Martin Thomas\r\n";
	unsigned short cnt = 0;
#if (F_CPU > 4000000UL)
#define CNTHALLO (unsigned int)(0xFFFF)
#else 
#define CNTHALLO (unsigned int)(0xFFFF/3)
#endif

	softuart_init();
	softuart_turn_rx_on(); /* redundant - on by default */
	
	sei();

	softuart_puts_P( "\r\nClixx.io Serial Terminal-Application\r\n" );    // "implicit" PSTR
	softuart_puts_p( PSTR("uses generic softuart driver code by Colin Gittins\r\n") ); // explicit PSTR
	softuart_puts_p( pstring ); // pstring defined with PROGMEM
	softuart_puts( "--\r\n$ " );  // string "from RAM"

#if WITH_STDIO_DEMO
	stdio_demo_func();
#endif

	setup();
	
	for (;;) {
	
		if ( softuart_kbhit() ) {
			c = softuart_getchar();
			softuart_putchar( c );
			buildbuffer( c );
		}
		
	}
	
	return 0; /* never reached */
}
