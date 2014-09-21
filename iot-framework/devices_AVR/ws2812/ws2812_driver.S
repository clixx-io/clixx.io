// ws2812 driver for attiny85
// Written by Mikael Lövqvist for David Lyon
// 2014-09-20


#include "ws2812_config.h"
#include <avr/io.h>
#define __SFR_OFFSET 0

.global ws2812_init, ws2812_send

#define OUT_HIGH sbi PORTB, WS2812_PIN
#define OUT_LOW cbi PORTB, WS2812_PIN

//Arguments
#define BUF_PTR r24
#define BUF_LEN r22

//Indirect registers (used as pointer for LED buffer)
#define X_POINTER r26

//Call clobbered registers r18 - r21 (used as vars here)
#define COUNT r18
#define BYTE r19
#define BIT r20
#define TMP r21

//branch if zero / not zero aliases to prevent brain meltdown during coding
#define brz breq
#define brnz brne

/*
800 kHz mode
bit 	cycles high/low 	times high/low
0		3, 7 				375 ns, 875 ns
1 		6, 4 				750 ns, 500 ns  

Note that 4 cycles low on 1 is not possible but most likely the ws2812 only cares about the positive pulse width (and it works fine)

400 kHz mode (not used here)
0		6, 14
1 		12, 8

*/

ws2812_init:
	cbi WS2812_PORT, WS2812_PIN
	sbi WS2812_PORTDIR, WS2812_PIN
	ret

ws2812_send:
	//Disable interrupts
	cli
	//Load arguments
	movw X_POINTER, BUF_PTR
	mov COUNT, BUF_LEN
	//Multiply with 3
	mov TMP, COUNT
	add COUNT, TMP
	add COUNT, TMP

	//Return if count is zero
	tst COUNT
	breq done

	buffer_loop:

		//Load data and reset bit counter
		ld BYTE, X+
		ldi BIT, 8

		bit_loop:

			//Check what bit to output
			sbrs BYTE, 7
			rjmp output_zero

		output_one:
			OUT_HIGH
			nop
			nop
			rol BYTE //Next bit
			dec BIT
			OUT_LOW

			//Is the byte done?
			brnz bit_loop
			rjmp next_byte

		output_zero:

			OUT_HIGH
			rol BYTE //Next bit
			OUT_LOW

			dec BIT   //1c 8tot
			//Is the byte done?
			brnz bit_loop

		next_byte:
			dec COUNT
			//Are we done?
			brne buffer_loop
	done:
			sei //Enable interrupts
			ret