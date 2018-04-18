/*
 =================================================================================
 * 
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
 * 
 =================================================================================
 */

#include <stdlib.h>
#include <stdio.h>

#ifdef TARGET_LINUX

  #include <signal.h>
  #include <unistd.h>
  #include <sys/time.h>
  #include <iostream>
  #include <fstream>
  using namespace std;
  
#endif

#include "clixxIO.hpp"

void *pMainClass;

/**********************************************************************
 * Setup a timer callback using the Linux Kernel
 *
 * @param interval - the number of seconds between callbacks
 * @return the CRC
 *
 **********************************************************************/  
int timer_setup (int interval)
{
  #ifdef TARGET_LINUX

    struct itimerval itimer;
  
    itimer.it_interval.tv_sec = interval;
    itimer.it_interval.tv_usec = 0;
    itimer.it_value.tv_sec = interval;	// interval value for timer 
    itimer.it_value.tv_usec = 0;
    int rc = setitimer(ITIMER_REAL, &itimer,0);
    signal(SIGALRM,timer_wakeup); 		// set the Alarm signal capture 
  
    return rc;
  #else
    return 1;
  #endif
 
}

/**********************************************************************
 * Handle a repeating interrupt using the Linux
 * Kernel
 *
 * @param interval	- The time in seconds between callbacks
 *
 **********************************************************************/  
void timer_wakeup (int interval)
{

#ifdef TARGET_LINUX
   signal(SIGALRM,timer_wakeup);
#endif

   //--Trigger the users callback   
   C_timerevent(pMainClass);
   
}

/**********************************************************************
 * Linux Exit function for termination signals.
 * 
 * This is called when the system has been notified that it must close down.
 *
 * @param i - Passed in by the Linux Kernel
 *
 **********************************************************************/  
void exit_func (int i)
{

#ifdef TARGET_LINUX

    signal(SIGINT,exit_func);
    printf("\nApplication shutdown signal - Shutting down\n");

#endif

    exit(0);
}

/**********************************************************************
 * 
 * Application Constructor
 * 
 **********************************************************************/  
clixxIOApp::clixxIOApp()
{
 
    // Initialise this variable
    setMainAppPtr((void *) this);
  
    #ifdef TARGET_LINUX
      // Setup a signal handler for exit
      signal(SIGINT,exit_func);

    #endif
  
}

/**********************************************************************
 * 
 * Application Destructor
 * 
 **********************************************************************/  
clixxIOApp::~clixxIOApp()
{
  
}

/**********************************************************************
 * 
 * Main method for allowing running the user to
 * run the application. All events are processed
 * here and sent to the application.
 * 
 **********************************************************************/  
int clixxIOApp::run()
{

    C_setupevent(pMainClass);
  
    puts("Application now in main loop");
  
    for (;;)
    {
        C_loopevent(pMainClass);

        #ifdef USE_MOSQUITTO
          if (IoT.available())
          {
            int rc(MOSQ_ERR_SUCCESS);
            do {
              rc = IoT.loop(-1);
            } while(rc == MOSQ_ERR_SUCCESS);
          }
        #endif
      
        if (Serial.available()) {
      
            char c = Serial.read();
    
            if (Serial.echo!=0)
                Serial.write(c);

            Serial.addbufferchar( c );
        }

    }
  
  return 0;
}

/**********************************************************************
 * 
 * UseConfig
 * Pin 
 *
 * @param configfile - pointer to the users configuration filename
 * 
 **********************************************************************/  
void clixxIOApp::useConfig(const char* configfile)
{
    #ifdef TARGET_LINUX
      _configfile = configfile;
    #endif
}

/**********************************************************************
 * 
 * Set the object address of the App class in main() 
 * Pointer to the address of the Users Application Object
 *
 * @param mainClass - pointer to the users Main App class
 * 
 **********************************************************************/  
void setMainAppPtr(void *mainClass)
{
  pMainClass = mainClass;

  printf("MainClass set to %p\n",mainClass);
}

/**********************************************************************
 * 
 * Setup a callback to a users looping event.
 * This will be called repeatedly from within
 * the application loop.
 *
 * @param function 	Pointer to the method to be called
 * @return 
 * 
 **********************************************************************/  
int addLoopEvent(void (*function)(int))
{
    return 0;
}

/**********************************************************************
 * 
 * Adds a Timer callback method to the callbacks.
 *
 * @param secs	the number of seconds between events
 * @return the CRC
 *
 * 
 **********************************************************************/  
int addTimerEvent(int secs, void (*function)())
{
  
  return timer_setup(secs);
  
}

/**********************************************************************
 * 
 * Setup a callback to a users IoT Subscrive event.
 * This will be called whenever an IoT packet is received
 * for a particular topic.
 *
 * @param function 	Pointer to the method to be called
 * @return 
 * 
 **********************************************************************/  
int addIoTSubEvent(const char *topic, void (*function)(void))
{
    #ifdef TARGET_AVR
      if (pMainClass == 0)
        return(-1);

      clixxIOApp *app = (clixxIOApp *) pMainClass;

      app->IoT.begin();

      app->IoT.echo = app->IoT.linemode = (unsigned char) 0x01; 

      app->IoT.write('#');
      app->IoT.puts(topic);
      app->IoT.write('\r');
    #endif
    
    return 0;
}

/**********************************************************************
 * 
 * Adds a PinChange callback method to the callbacks.
 *
 * @param length	the length of the data string
 * @return the CRC
 * 
 **********************************************************************/  
int addPinChangeEvent(int pin, int changetype, void (*function)())
{
    return 1;
}

/**********************************************************************
 * 
 * An efficient routine to convert a decimal to a string
 *
 * @param Unsigned to convert
 * @param Pointer to the end of the string
 * 
 * @return the CRC
 * 
 **********************************************************************/  
char *dec(unsigned x, char *s)
{
    *--s = 0;
    if (!x) *--s = '0';
    for (; x; x/=10) *--s = '0'+x%10;
    return s;
}

/**********************************************************************
 * PWM write to a GPIO Pin
 * 
 * This is software PWM function that oscilates a pin at a particular
 * percentage value for a time measured in 1/10ths of a second.
 *
 * @param onpercentage - A value between 0 - 100 representing the number
 *                       of on cycles in 100 for the pin to pulse. A
 *                       value of 10 will give a 10% duty cycle.
 * 
 * @param seconds     - number of seconds for the function to run
 * @param deciseconds - number of 1/10th second units to run.
 *
 **********************************************************************/  
void clixxIOGPIOPin::pwmWrite(short onpercentage,int seconds, int deciseconds)
{
    unsigned short on_marks = 1;
    unsigned short off_marks = (100 / onpercentage) - 1;
    
    unsigned long ds = (seconds * 10) + deciseconds;
    unsigned long oncount=0,offcount=0;

    // onpercentage's are translated down to simple values
    switch (onpercentage)
    {
        case 0 :  on_marks = 0;
                  off_marks = 10;
                  break;
                 
        case 1 : 
                  on_marks = 1;
                  off_marks = 99;
                  break;
        case 2 : 
                  on_marks = 1;
                  off_marks = 49;
                  break;
        case 3 : 
        case 4 : 
        case 5 :  on_marks = 1;
                  off_marks = 19;
                  break;
                 
        case 6  : 
        case 7  : 
        case 8  : 
        case 9  : 
        case 10 : on_marks = 1;
                  off_marks = 9;
                  break;

        case 20 : on_marks = 1;
                  off_marks = 4;
                  break;

        case 25 : on_marks = 1;
                  off_marks = 3;
                  break;
                 
        case 33 : on_marks = 1;
                  off_marks = 3;
                  break;
                 
        case 40 : on_marks = 4;
                  off_marks = 6;
                  break;

        case 50 : on_marks = 1;
                  off_marks = 1;
                  break;

        case 66: 
        case 67 : on_marks = 2;
                  off_marks = 1;
                  break;

        case 70 : on_marks = 7;
                  off_marks = 3;
                  break;

        case 75 : on_marks = 3;
                  off_marks = 1;
                  break;
                 
        case 80 : on_marks = 4;
                  off_marks = 1;
                  break;

        case 90 : on_marks = 9;
                  off_marks = 1;
                  break;

        case 100: on_marks = 10;
                  off_marks = 0;
                  break;
                  
        default:  // ToDo - Make less common values calculate correctly
                  on_marks = 0;
                  off_marks = 1;
    }
   
    // Calculate how many marks will fit into one decisecond
    unsigned short dc_repeats = 100 / (on_marks + off_marks);
    
    // We write to a log file for testing/validation
    #ifdef TARGET_LINUX
      ofstream trace_file;
      trace_file.open ("pwmWrite.log");      
      trace_file << "pwmwrite running for " << onpercentage << "%\n";
      trace_file << "pwmwrite running for "  << seconds+deciseconds << "deciseconds\n";
      trace_file << "pwmwrite will be off " << off_marks << " and on " << on_marks << " for every cycle, total=" << (on_marks+off_marks) << '\n';
      trace_file << "dc_repeats= " << dc_repeats << '\n';
      trace_file << "ds = " << ds << '\n';
    #endif
    
    for (unsigned long dc = 0; dc < ds; dc++)
    {
        
        for (short d=0; d<dc_repeats; d++)
        {
            
            // Writing high pulse values
            for (unsigned short e=0; e < on_marks; e++)
            {
                this->digitalWrite(true);
            
                delay_ms(1);
                oncount++;
                
                #ifdef TARGET_LINUX
                  trace_file << "On \n";
                #endif
            }
            
            // Writing low pulse values
            for (unsigned short e=0; e < off_marks; e++)
            {
                
                this->digitalWrite(false);
                delay_ms(1);

                offcount++;
                
                #ifdef TARGET_LINUX
                  trace_file << "Off \n";
                #endif
                
            }
           
        }
        
        #if defined(TARGET_LINUX)
          trace_file << "oncount=" << oncount << ",offcount=" << offcount << "\n";
        #endif
        
    }

    #ifdef TARGET_LINUX
      trace_file.close();
    #endif
}

/**********************************************************************
 * Milisecond delay
 * 
 * This is called when the system needs to pause for time.
 *
 * @param ms - miliseconds to pause
 *
 **********************************************************************/  
void delay_ms(unsigned long ms)
{

    #if defined(__WIN32__) || defined(_WIN32) || defined(WIN32) || defined(__WINDOWS__) || defined(__TOS_WIN__)
      Sleep( ms ); 
    #elif defined(TARGET_LINUX)  /* presume POSIX */
      usleep( ms * 1000 ); 
    #elif defined(TARGET_AVR)
      for (unsigned long x = 0; x < ms; x++)
      {
          _delay_ms(1); 
      }
    #endif 
    
}
