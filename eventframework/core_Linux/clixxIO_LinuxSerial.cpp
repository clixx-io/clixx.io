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

extern int serial_feed_setup(const char *portname, long baudrate);
extern int serial_feed_capture(int tty_fd, char *buffer, int buffersize, int dumpchars);
extern int serial_feed_close(int tty_fd);
extern int serial_available(int tty_fd);
extern int term_setup(int fd, long baudrate);

clixxIOSerial::clixxIOSerial()
{
	fd = -1;
}

int clixxIOSerial::begin(const char *portname, long baudrate)
{
	return serial_feed_setup(portname, baudrate);
}

void clixxIOSerial::end()
{
	serial_feed_close(fd);
}

int clixxIOSerial::available(void)
{
	return serial_available(fd);
}

unsigned char clixxIOSerial::read(void)
{
	unsigned char rx_buff;

	if (0==serial_feed_capture(fd, (char *) &rx_buff, 1))
		return rx_buff;
	else
		return (unsigned char ) 0;
}

void clixxIOSerial::flush(void)
{
}
