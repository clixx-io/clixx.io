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
#include <cstring>
#include "clixxIO.hpp"

extern int serial_feed_setup(const char *portname, long baudrate);
extern int serial_feed_capture(int tty_fd, char *buffer, int buffersize, int dumpchars);
extern int serial_feed_close(int tty_fd);
extern int serial_available(int tty_fd);
extern int term_setup(int fd, long baudrate);
extern int serial_puts(int tty_fd, const char *output);
extern int serial_putc(int tty_fd, const char outputc);

clixxIOSerial::clixxIOSerial()
{
    fd = -1;
}

/** Initialise the Serial port at a specific Baud Rate
*
* This function initialises the serial port as well as setting up the input
* pin attached to the Serial port. The set up accepts portname as the Windows
* comport name : ie "COM1" or in Linux "/dev/ttyUSB0". The Baudrate is
* optional. On some platforms like the Attiny85 with softuart it's fixed
* to a precompiled vale and can't be changed.
* 
* Assume 8N1 for all port settings.
*
* @portname 	the filename of the port to initialise.
* 
* @baudrate		baudrate to run the port. 
*/
int clixxIOSerial::begin(const char *portname, long baudrate)
{
    return serial_feed_setup(portname, baudrate);
}

/** End using the Serial port and close it
*
* This function ends using the serial port.
*/
void clixxIOSerial::end()
{
    serial_feed_close(fd);
}

/** Determines if characters are available for reading
*
* This function checks internal buffers and informs the
* caller if there are characters that are available for
* reading from the port.
* 
*/
int clixxIOSerial::available(void)
{
    return serial_available(fd);
}

/** Reads a character from the serial port
*
* This function reads one character from the serial port
* if available.
* 
*/
unsigned char clixxIOSerial::read(void)
{
    unsigned char rx_buff;

    if (0==serial_feed_capture(fd, (char *) &rx_buff, 1))
        return rx_buff;
    else
        return (unsigned char ) 0;
}

/** Flushes the serial port Buffer
*
* This function reads one character from the serial port
* if available.
* 
*/
void clixxIOSerial::flush(void)
{
}

/** Writes a character to the serial port
*
* This function writes one character to the serial port.
* 
*/
int clixxIOSerial::write(const char c)
{
	return serial_putc(fd, (const unsigned char ) c);
}

/** Writes a string to the serial port
*
* This function writes a string to the serial port
* 
*/
int clixxIOSerial::puts(const char *s)
{

	return serial_puts(fd, s);
}

