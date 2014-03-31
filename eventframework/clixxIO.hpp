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

int addLoopEvent(void (* function)(int));
int addTimerEvent(int secs, void (*function)());
int addPinChangeEvent(int pin, int changetype, void (*function)(int,int));
int addSerialInterruptEvent(int secs, void (*function)(int,int));

void setMainAppPtr(void *mainClass);
void C_timerevent( void* appC);
void C_startupevent( void* appC);

class clixxIOApp{

    private:

     unsigned int (*loopmethod)(unsigned int);
     unsigned int (*timermethod)(unsigned int);
     
    public:
     int run();
     
     clixxIOApp();
     ~clixxIOApp();
};

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

  #include <windows.h>
  inline void delay( unsigned long ms ){ Sleep( ms ); }

#else  /* presume POSIX */

  #include <unistd.h>

  inline void delay( unsigned long ms ){ usleep( ms * 1000 ); }

#endif 

extern int serial_feed_setup(const char *portname);
extern int serial_feed_capture(int tty_fd, char *buffer, int buffersize, int dumpchars = 0);
extern int serial_feed_close(int tty_fd);

/*
  HardwareSerial.h - Hardware serial library for Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 17 March     2014 by David Lyon
  Modified 28 September 2010 by Mark Sproul
*/

#include <inttypes.h>

// #include "core_build_options.h"
// #include "Stream.h"

struct ring_buffer;

#ifdef Stream
class clixxIOSerial : public Stream
#else
class clixxIOSerial
#endif
{
  private:
    ring_buffer *_rx_buffer;
    volatile uint8_t *_ubrrh;
    volatile uint8_t *_ubrrl;
    volatile uint8_t *_ucsra;
    volatile uint8_t *_ucsrb;
    volatile uint8_t *_udr;
    uint8_t _rxen;
    uint8_t _txen;
    uint8_t _rxcie;
    uint8_t _udre;
    uint8_t _u2x;
    #ifdef PLATFORM_LINUX
    int fd;
    #endif
    
  public:
    clixxIOSerial(ring_buffer *rx_buffer,
      volatile uint8_t *ubrrh, volatile uint8_t *ubrrl,
      volatile uint8_t *ucsra, volatile uint8_t *ucsrb,
      volatile uint8_t *udr,
      uint8_t rxen, uint8_t txen, uint8_t rxcie, uint8_t udre, uint8_t u2x);
    void begin(long);
    void end();
    virtual int available(void);
    virtual int peek(void);
    virtual int read(void);
    virtual void flush(void);
    virtual size_t write(uint8_t);
#ifdef Stream
    using print::write; // pull in write(str) and write(buf, size) from Print
#endif
};

/*
 * General i2c bus class. Correspends to a physical 
 *
 */
class clixxIO_I2C_bus {
	
	clixxIO_I2C_bus(int bus = 1);
	
	public:
		int i2c_file;	
};
/*
 * General i2c device class so that other devices can be added easily
 *
 */
class clixxIO_I2C_device {

  public:
	clixxIO_I2C_device(int addr, int port);

    int write(char byte);
    char read();
    int read_nbytes_data(char *data, int n);

};


#if (defined(UBRRH) || defined(UBRR0H)) && ! DEFAULT_TO_TINY_DEBUG_SERIAL
  extern HardwareSerial Serial;
#elif defined(USBCON)
  #include "usb_api.h"
#endif
#if defined(UBRR1H)
  extern HardwareSerial Serial1;
#endif
#if defined(UBRR2H)
  extern HardwareSerial Serial2;
#endif
#if defined(UBRR3H)
  extern HardwareSerial Serial3;
#endif

