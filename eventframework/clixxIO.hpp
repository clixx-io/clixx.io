
class clixxIOApp{

    private:
     // unsigned static int (*loopmethod)(unsigned int);

     unsigned int (*loopmethod)(unsigned int);
     unsigned int (*timermethod)(unsigned int);
     
    public:
     int run();
     int addLoopEvent(void (*function)(int));
     int addTimerEvent(int secs, void (*function)(int,int));
     int addInterruptEvent(int secs, void (*function)(int,int));
     int addSerialInterruptEvent(int secs, void (*function)(int,int));
     
     clixxIOApp();
     ~clixxIOApp();
};

