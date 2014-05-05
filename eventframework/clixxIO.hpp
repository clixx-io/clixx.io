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
void C_loopevent( void* appC);
void C_timerevent( void* appC);
void C_setupevent( void* appC);
void C_shutdownevent( void* appC);
void C_serialevent( void* appC);
void C_iotevent( void* appC);

class clixxIOApp{

  private:
      
    unsigned int (*loopmethod)(unsigned int);
    unsigned int (*timermethod)(unsigned int);
    unsigned int (*serialmethod)(unsigned int);
    unsigned int (*iotmethod)(unsigned int);
     
  public:
    unsigned int serial_debug_msgs;

    int run();
     
    clixxIOApp();
    ~clixxIOApp();
};

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

  #include <windows.h>
  inline void delay( unsigned long ms ){ Sleep( ms ); }

#elif defined(TARGET_LINUX)  /* presume POSIX */

  #include <unistd.h>
  #include <iostream>
  #include <mosquittopp.h>

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
// #include "Stream.h"

#define BUFFSIZE_LINELEN 60		// Defines a Serial-line buffer size

#ifdef Stream
class clixxIOSerial : public Stream
#else
class clixxIOSerial
#endif
{
  private:	
  #ifdef TARGET_LINUX
    int fd;
  #endif
    unsigned char linebuffer[BUFFSIZE_LINELEN];
    unsigned char linebufferpos;
    
  public:
    clixxIOSerial();
    int begin(const char *portname = 0, long baudrate = 0);
    void end();
    virtual int available(void);
    virtual unsigned char read(void);
    virtual void flush(void);
    virtual int write(unsigned char);
    virtual int puts(const char *);
#ifdef Stream
    using print::write; // pull in write(str) and write(buf, size) from Print
#endif

};

/*
 * General i2c bus class. Correspends to a physical 
 *
 */
class ClixxIO_I2cBus {

  public:
  
    ClixxIO_I2cBus(int bus = 1);

    int write(unsigned char addr, unsigned char reg, unsigned char byte);
    char read(unsigned char addr, unsigned char reg, unsigned char *val);
    int read_nbytes_data(unsigned char addr, char *data, int n);

  private:
    int i2c_file;	
};
/*
 * General i2c device class so that other devices can be added easily
 *
 */
#ifdef TARGET_LINUX
class ClixxIO_IoTSub : public mosquittopp::mosquittopp 
#else
class ClixxIO_IoTSub : public clixxIOSerial
#endif
{
	public:
		ClixxIO_IoTSub(const char* id);
		virtual ~ClixxIO_IoTSub();

	private:
		uint16_t mid;

		void on_connect(int rc);
		void on_subscribe(uint16_t mid, int qos_count, const uint8_t *granted_qos);
		void on_message(const struct mosquitto_message *message);
		void print_error_connection(int rc);
};

class ClixxIO_I2cDevice {

  public:
    ClixxIO_I2cDevice();
    ClixxIO_I2cDevice(int deviceAddress, int bus = 1);

  protected:
    ClixxIO_I2cBus *i2cbus;
    int device;	
};

extern clixxIOSerial Serial;

#if defined(UBRR1H)
  extern HardwareSerial Serial1;
#endif
#if defined(UBRR2H)
  extern HardwareSerial Serial2;
#endif
#if defined(UBRR3H)
  extern HardwareSerial Serial3;
#endif

/* ----------------------------------------------------------------------------
   #  Description
   ---------------------------------------------------------------------------- */
class ClixxIO_i2cLCD : public ClixxIO_I2cDevice {

public:
  // Delay times
  static const double DELAY;
  static const double PULSE;

  // MCP23008 Register Numbers
  static const int IODIR;
  static const int IPOL;
  static const int GPPU;
  static const int GPIO;

  // Command bits
  static const int LCD_ENABLE; // Enable line
  static const int LCD_COMMAND; // Command/Data select
  static const int LCD_DATA; // Data bits

  // Addresses
  static const int LCD_LINE_1;  // LCD RAM address for the 1st line
  static const int LCD_LINE_2;  // LCD RAM address for the 2nd line

  public:
  
    ClixxIO_i2cLCD(int bus = 1, int device = 0x22);
    
  	void _writeLCD(unsigned char value, bool cmd = false);
  	void _update();
  	void setup();
    void gotoXY(int x, int y);
    void write(const char *text);
    void writeline(int lineno, const char *text);
    void clear();
    
  protected:
    ClixxIO_I2cBus *i2cbus;
    char lines[2][16];
    unsigned int where[2];

};

#pragma GCC diagnostic ignored "-Wpmf-conversions"
 
