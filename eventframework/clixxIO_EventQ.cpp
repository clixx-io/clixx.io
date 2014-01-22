#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#include "clixxIO.hpp"

#define INTERVAL 2

int howmany = 0;

class clixxIOTimer {
  public:
  private:
    int durationmSecs;
};


class clixxIOMsgQ {

  public:                    // begin public section
    clixxIOMsgQ(int initialAge);     // constructor
    ~clixxIOMsgQ();                  // destructor
    
//    int peekMsg() const;
//    int getMsg();
    
    int run();  // Main event for running
    
 private:                   // begin private section
    int itsAge;              // member variable

    int eventlist[10];
    
};

// constructor of Cat,
clixxIOMsgQ::clixxIOMsgQ(int initialAge)
{
  itsAge = initialAge;
}

clixxIOMsgQ::~clixxIOMsgQ()                 // destructor, just an example
{
}

// GetAge, Public accessor function
// returns value of itsAge member
int clixxIOMsgQ::run()
{
   return itsAge;
}

void timer_wakeup (int interval)
{
   struct itimerval tout_val;

   signal(SIGALRM,timer_wakeup);

   howmany += INTERVAL;
   
   printf("\n%d sec up partner, Wakeup!!!\n",howmany);
   tout_val.it_interval.tv_sec = 0;
   tout_val.it_interval.tv_usec = 0;
   tout_val.it_value.tv_sec = interval; /* 10 seconds timer */
   tout_val.it_value.tv_usec = 0;
   
   setitimer(ITIMER_REAL, &tout_val,0);
   
}

void timer_setup (int interval)
{
  struct itimerval tout_val;
  
  tout_val.it_interval.tv_sec = 0;
  tout_val.it_interval.tv_usec = 0;
  tout_val.it_value.tv_sec = INTERVAL;  // 10 seconds timer 
  tout_val.it_value.tv_usec = 0;
  setitimer(ITIMER_REAL, &tout_val,0);
  signal(SIGALRM,timer_wakeup); // set the Alarm signal capture 
 
}


void exit_func (int i)
{
    signal(SIGINT,exit_func);
    printf("\nBye Bye!!!\n");
    exit(0);
}

int main0 ()
{
  
  signal(SIGINT,exit_func);
  
  timer_setup(2);
  
  while (1)
  {
    //printf("!");
  }
  
  return 0;

}

clixxIOApp::clixxIOApp()
{
  
  signal(SIGINT,exit_func);
 
}

clixxIOApp::~clixxIOApp()
{
  
}

int clixxIOApp::run()
{ 
  for (;;)
    if (loopmethod != NULL)
      loopmethod(1);
}

int clixxIOApp::addLoopEvent(void (*function)(int))
{

}

int clixxIOApp::addTimerEvent(int secs, void (*function)(int))
{
  
  timer_setup(secs);
  
}

int clixxIOApp::addInterruptEvent(int secs, void (*function)(int,int))
{
  
}

int clixxIOApp::addSerialInterruptEvent(int secs, void (*function)(int,int))
{
  
}
