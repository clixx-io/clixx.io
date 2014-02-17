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
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "clixxIO.hpp"

int howmany = 0;
void *pMainClass;	

/** ---------------------------------------------------------------
 * Message Queue Class for passing messages to the
 * application.
 *
 */
class clixxIOMsgQ {

  public:                    	// begin public section
    clixxIOMsgQ();     		// constructor
    ~clixxIOMsgQ();          	// destructor
    
    int peekMsg() const;
    int getMsg();
    
    int run();  				// Main event for running
    
 private:                   	// begin private section

    int eventlist[10];
    
};

/**
 * Message Queue Constructor
 */
clixxIOMsgQ::clixxIOMsgQ()
{
}

/**
 * Message Queue Destructor
 */
clixxIOMsgQ::~clixxIOMsgQ()                 // destructor, just an example
{
}

/**
 * Message Queue Run Method
 */
int clixxIOMsgQ::run()
{
	return 0;
}

/**
 * Handle a repeating interrupt using the Linux
 * Kernel
 *
 * @param interval	- The time in seconds between callbacks
 */
void timer_wakeup (int interval)
{

   signal(SIGALRM,timer_wakeup);

   //--Trigger the users callback   
   C_timerevent(pMainClass);
   
}

/**
 * Setup a timer callback using the Linux Kernel
 *
 * @param interval - the number of seconds between callbacks
 * @return the CRC
 */
void timer_setup (int interval)
{
  struct itimerval itimer;
  
  itimer.it_interval.tv_sec = interval;
  itimer.it_interval.tv_usec = 0;
  itimer.it_value.tv_sec = interval;  // 10 seconds timer 
  itimer.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &itimer,0);
  signal(SIGALRM,timer_wakeup); 		// set the Alarm signal capture 
 
}

/**
 * Linux Exit function for termination signals.
 * 
 * This is called when the system has been notified that it must close down.
 *
 * @param i - Passed in by the Linux Kernel
 */
void exit_func (int i)
{
    signal(SIGINT,exit_func);
    printf("\nApplication shutdown signal - Shutting down\n");
    exit(0);
}

/**
 * Application Constructor
 * 
 */
clixxIOApp::clixxIOApp()
{
  
  // Setup a signal handler for exit
  signal(SIGINT,exit_func);
  
}

/**
 * Application Destructor
 * 
 */
clixxIOApp::~clixxIOApp()
{
  
}

/**
 * Main method for allowing running the user to
 * run the application. All events are processed
 * here and sent to the application.
 *
 */
void clixxIOApp::run()
{
	
  C_startupevent(pMainClass);
  
  puts("Application now in main loop");
  
  for (;;);
  
}

/**
 * Set the object address of the App class in main() 
 * Pointer to the address of the Users Application Object
 *
 * @param mainClass - pointer to the users Main App class
 */
// 
// 
void setMainAppPtr(void *mainClass)
{
  pMainClass = mainClass;	

  printf("MainClass set to %p\n",mainClass);
}

/**
 * Setup a callback to a users looping event.
 * This will be called repeatedly from within
 * the application loop.
 *
 * @param function 	Pointer to the method to be called
 * @return 
 */
int addLoopEvent(void (*function)(int))
{
	return 0;
}

/**
 * Adds a Timer callback method to the callbacks.
 *
 * @param secs	the number of seconds between events
 * @return the CRC
 */
int addTimerEvent(int secs, void (*function)())
{
  
  timer_setup(secs);
  
}

/**
 * Adds a PinChange callback method to the callbacks.
 *
 * @param length	the length of the data string
 * @return the CRC
 */
int addPinChangeEvent(int pin, int changetype, void (*function)())
{
	return 0;
}

/**
 * Adds a SerialInterrupt callback method to the callbacks.
 *
 * @param length	the length of the data string
 * @return the CRC
 */
int addSerialInterruptEvent(int secs, void (*function)())
{
  
}

