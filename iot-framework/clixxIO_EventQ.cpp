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
          if (IoT.available() 
          {
            do {
              rc = mqttc->loop(-1);
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
    const short skip_marks = (100 / onpercentage) - 1;
    
    unsigned long ds = (seconds * 10) + deciseconds;
    unsigned long oncount=0,offcount=0;
    
    #if defined(TARGET_LINUX)
      printf("pwmwrite running for %d deciseconds\n",seconds+deciseconds);
    #endif
    
    for (unsigned long dc = 0; dc < ds; dc++)
    {
        short e=skip_marks;
        
        for (short d=0; d<100; d++)
        {
            
            this->digitalWrite(true);
            
            delay_ms(1);
            oncount++;
            
            if (e > 0)
            {
                if (e-- != 0)
                {
                    this->digitalWrite(false);
                }
                
                delay_ms(1);
                offcount++;
                
            } else
            {
                e = skip_marks;
            }
            
            #if defined(TARGET_LINUX)
              printf("oncount= %lu offcount =%lu\n",oncount,offcount);
            #endif
        }
        
    }

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
