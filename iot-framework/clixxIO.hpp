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

#include <stdlib.h>
#include <inttypes.h>

#if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)

  #include <windows.h>
  #include "Stream.h"

#elif defined(TARGET_LINUX)  /* presume POSIX */

  #include <unistd.h>
  #include <iostream>
  #include <mosquittopp.h>
  #include <string>
  #include <sstream>
  #include <errno.h>
  #include <stdio.h>
  #include <sys/types.h>
  #include <sys/stat.h>
  #include <fcntl.h>
  #include "Stream.h"

  using namespace std;

#endif 

extern void *pMainClass;
void setMainAppPtr(void *mainClass);

extern char *dec(unsigned x, char *s);

#define BUFFSIZE_LINELEN 60		// Defines a Serial-line buffer size
#define BUFFSIZE_IOTTOPICLEN 30		// Defines a Serial-line buffer size

/* -------------------------------------------------------------------------
 *
 * Serial-Port Handling Class.
 *
 * ------------------------------------------------------------------------*/
#ifdef Stream
class clixxIOSerial : public Stream
#else
class clixxIOSerial
#endif
{
  public:
  
    unsigned short linemode;
    unsigned short echo;
    unsigned short iotmode;
    
    clixxIOSerial();
    int begin(const char *portname = 0, long baudrate = 0);
    void end();
    virtual int available(void);
    virtual unsigned char read(void);
    virtual void flush(void);
    int write(const char);
    int puts(const char *);

    inline void print(const char c){ write(c); };
    inline void print(const char *s){ puts(s); };
    inline void print(const int i){ char buf[10]; char *b=dec(i, (char *) &buf); puts((char *) b); };
    // inline int print(const float f){static char buf[10]; sprintf(buf, "%.2f", f); puts(buf)};
    void println(const char *);
    void println(const float f);
    
    const char *lastline() { return (const char *) &linebuffer; };
    
    // Some IoT methods
    const char *iotpacket() { return (const char *) &linebuffer; };
    int beginPublishing(const char *topic);

    int publish(const char *publishtext);
    int publish(const int publishint);
    
    virtual void addbufferchar(char);
    virtual void processcommand(void);
    
#ifdef Stream
    using print::write; // pull in write(str) and write(buf, size) from Print
#endif

  private:
  #ifdef TARGET_LINUX
    int fd;
  #endif
    char linebuffer[BUFFSIZE_LINELEN];
    int linebufferpos;

};

extern clixxIOSerial Serial;
#ifdef TARGET_LINUX
 #define Debug Serial 
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

#if defined(TARGET_LINUX)  /* presume POSIX */
extern int serial_feed_setup(const char *portname);
extern int serial_feed_capture(int tty_fd, char *buffer, int buffersize, int dumpchars = 0);
extern int serial_feed_close(int tty_fd);
#endif 

/* -------------------------------------------------------------------------
 *
 * IoT Subscriber Class. Inspired by Mosquitto
 *
 * ------------------------------------------------------------------------*/
#ifdef TARGET_LINUX
class ClixxIO_IoTSub : public mosquittopp::mosquittopp 
#else
class ClixxIO_IoTSub : public clixxIOSerial
#endif
{
  public:

    #ifdef TARGET_LINUX
    ClixxIO_IoTSub(const char* id);
    ~ClixxIO_IoTSub();
    int connect(const char *host, int port=1883, int keepalive=60, bool clean_session=true);
    int subscribeto(const char* topic);
    #else
    ClixxIO_IoTSub(const char* id);
    ~ClixxIO_IoTSub();
    int connect();
    int subscribeto(const char* topic);
    #endif

    int disconnect();

  private:
    char _topic[BUFFSIZE_IOTTOPICLEN];
    char _host[];
    int  _port, _keepalive;
    bool _clean_session;

    #ifdef TARGET_LINUX
    uint16_t mid;

    void on_connect(int rc);
    void on_subscribe(uint16_t mid, int qos_count, const uint8_t *granted_qos);
    void on_message(const struct mosquitto_message *message);
    void print_error_connection(int rc);
    #endif
};

/* -------------------------------------------------------------------------
 *
 * clixxIOApp. A basic class for encapsulating the runtime system.
 * 
 *             To be inherited by the application
 *
 * ------------------------------------------------------------------------*/
class clixxIOApp{

  public:

    clixxIOApp();
    ~clixxIOApp();

    void useConfig(const char* configfile);
    int run();

    #ifdef TARGET_AVR
      clixxIOSerial IoT;
    #endif
    #ifdef TARGET_LINUX
      #if defined(USE_MOSQUITTO)
        ClixxIO_IoTSub IoT;
      #else
        clixxIOSerial IoT;
      #endif
    #endif

    #if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
      inline void delay_ms( unsigned long ms ){ Sleep( ms ); }
    #elif defined(TARGET_LINUX)  /* presume POSIX */
      inline void delay_ms( unsigned long ms ){ usleep( ms * 1000 ); }
    #elif defined(TARGET_AVR)
    #endif 
    
  protected:
    #ifdef TARGET_LINUX
      string _configfile;
    #endif
 
};

/* -------------------------------------------------------------------------
 *
 * GPIO/Pin Functions. Inspired by Wiring
 *
 * ------------------------------------------------------------------------*/
#define OUTPUT 1
#define INPUT  0
#define HIGH   1
#define LOW    0

void pinMode(int pin, int value);
void digitalWrite(int pin, int value);
int  digitalRead(int pin);
void analogWrite(int pin, int value);
int  analogRead(int pin);
/* ------------------------------------------------------------------------*/

#if defined(TARGET_AVR)  

  #include <avr/io.h>

  // Some methods to quickly return  
  int getInputPin(int portname);
  int getOutputPin(int portname);
  
  enum { DIGITAL_1_PORT, DIGITAL_2_PORT };
  
#else
  int getInputPin(int portname);
  int getOutputPin(int portname);
#endif

/* -------------------------------------------------------------------------
 *
 * GPIO Pin Class
 * 
 * Purpose: Each object instatiated from this class will control a GPIO pin
 * The GPIO pin number must be passed to the overloaded class constructor
 *
 * ------------------------------------------------------------------------*/
class clixxIOGPIOPin
{
  public:
    clixxIOGPIOPin();
    clixxIOGPIOPin(short pinnumber,short direction=0);
    ~clixxIOGPIOPin();

    #if defined(TARGET_LINUX)  	
      /* String types supported are supported for GPIO read/writes */
      clixxIOGPIOPin(string pinnumber);
      clixxIOGPIOPin(clixxIOApp *app, string devicename);
      int setdir(string dir);
      int setval(string val);
      string getpinnumber();
    #endif

    void digitalWrite(short val);
    int digitalRead();
    int configure(const char *logicalname);
    int configure(short pinnumber,short direction=0);
 
    void pwmWrite(short onpercentage,int seconds, int deciseconds=0);
    
  protected:
    #if defined(TARGET_LINUX)
      string _gpionum;
    #else
      short _gpionum;
      short _direction;
    #endif
    
  private:
    #if defined(TARGET_LINUX)  	
      int exportpin();
      int unexportpin();

      int valuefd;
      int directionfd;
      int exportfd;
      int unexportfd;
    #endif

};

/* -------------------------------------------------------------------------
 *
 * I2C Access Class. Provides a bus and Device.
 *
 * ------------------------------------------------------------------------*/
class ClixxIO_I2cBus {

  private:
    int i2c_file;

  public:

    ClixxIO_I2cBus(int bus = 1): i2c_file(bus) { };

    int connect(const int bus);
    void disconnect();
    
    int write(unsigned char addr, unsigned char reg, unsigned char byte);
    char read(unsigned char addr, unsigned char reg, unsigned char *val);
    int read_nbytes_data(unsigned char addr, char *data, int n);

};

/*
 * General i2c device class so that other devices can be added easily
 *
 */
class ClixxIO_I2cDevice {

  public:
    ClixxIO_I2cDevice();
    ClixxIO_I2cDevice(int deviceAddress, int bus = 1);

  protected:
    ClixxIO_I2cBus *i2cbus;
    int device;	
};

/* -------------------------------------------------------------------------
 *
 * Classes to Support the Basic clixx.io boards
 *
 * ------------------------------------------------------------------------*/
class clixxIO_Button : public clixxIOGPIOPin {

  public:

    bool pressed();

};

class clixxIO_Switch : public clixxIOGPIOPin {

  public:
    
    void On();  
    void Off();

};

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

#ifdef TARGET_AVR
 #define Debug IoT
#endif

#pragma GCC diagnostic ignored "-Wpmf-conversions"
 
/* -------------------------------------------------------------------------
 *
 * EventFramework Control
 *
 * ------------------------------------------------------------------------*/
/*
 * setup() construct
 *
 */
void C_setupevent( void* appC);
 
/*
 * loop() construct
 *
 */
int addLoopEvent(void (* function)(int));
void C_loopevent( void* appC);

/*
 * timer() construct
 *
 */
int addTimerEvent(int secs, void (*function)());
void C_timerevent( void* appC);
void timer_wakeup (int interval);

/*
 * pin() construct
 *
 */
int addPinChangeEvent(int pin, int changetype, void (*function)(int,int));
void C_pinchange( void* appC);

/*
 * shutdown() construct
 *
 */
int addShutdownEvent(void (*function)(int,int));
void C_shutdownevent( void* appC);

/*
 * Serial Callbacks
 *
 */
int addSerialCharEvent(void (*function)(int,int));
int addSerialLineEvent(void (*function)(int,int));
int addSerialOpenEvent(void (*function)(int,int));
int addSerialCloseEvent(void (*function)(int,int));
void C_serialchar( void* appC);
void C_serialline( void* appC);
void C_serialopen( void* appC);
void C_serialclose( void* appC);

int addIoTSubEvent(const char *topic, void (*function)(void));
int addIoTOpenEvent(void (*function)(int,int));
int addIoTCloseEvent(void (*function)(int,int));
void C_iotmessage( void* appC);
void C_iotopen( void* appC);
void C_iotclose( void* appC);
