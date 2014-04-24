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
#include "clixxIO.hpp"
#include "core_AVR/softuart.h"

clixxIOSerial::clixxIOSerial(unsigned char *rx_buffer, uint8_t rx_len)
{
	
}

int clixxIOSerial::begin(const char *portname, long baudrate)
{
	// Setting the baud rate isn't supported if we are using softuart

	softuart_init();

	softuart_turn_rx_on();

	return;
}

void clixxIOSerial::end()
{
	softuart_turn_rx_off();
	
	return;
}

int clixxIOSerial::available(void)
{
	return softuart_kbhit();
}

unsigned char clixxIOSerial::read(void)
{
	// Return a character
	return softuart_getchar();
}

void clixxIOSerial::flush(void)
{
	// flush the buffer
	softuart_flush_input_buffer();
}

int clixxIOSerial::write(const unsigned char c)
{
	softuart_putchar(c);
	
	return 0;
}

int clixxIOSerial::puts(const char *s)
{
	softuart_puts( s );
	
	return 0;
}
