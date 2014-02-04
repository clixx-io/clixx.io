
int addLoopEvent(void (* function)(int));
int addTimerEvent(int secs, void (*function)());
int addInterruptEvent(int secs, void (*function)(int,int));
int addSerialInterruptEvent(int secs, void (*function)(int,int));

void setMainAppPtr(void *mainClass);
void C_timerevent( void* appC);
void C_startupevent( void* appC);

class clixxIOApp{

    private:
     // unsigned static int (*loopmethod)(unsigned int);

     unsigned int (*loopmethod)(unsigned int);
     unsigned int (*timermethod)(unsigned int);
     
    public:
     int run();
     
     clixxIOApp();
     ~clixxIOApp();
};

