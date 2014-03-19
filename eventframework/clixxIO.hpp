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

class clixxIOSerial(){

	
}

extern int serial_feed_setup(const char *portname);
extern int serial_feed_capture(int tty_fd, char *buffer, int buffersize, int dumpchars = 0);
extern int serial_feed_close(int tty_fd);
