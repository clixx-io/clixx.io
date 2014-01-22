#include <iostream> 

class clixxIO_App { 
     public: 
     virtual void loop() const; 
     typedef void (clixxIO_App::*loopPtr)() const; 
     
}; 

class myApp: public clixxIO_App { 
    
    public: 
    
    virtual void loop() const 
    { 
		std::cout << "I am the loop() - Derived" << std::endl; 
	}
    
}; 

void clixxIO_App::loop() const 
{ 
	std::cout << "I am the loop - clixxIO_App" << std::endl; 
} 

int main( int argc, char **argv ) {
     
     clixxIO_App::loopPtr func = &clixxIO_App::loop; 
     
     clixxIO_App theBase; 
     
     theBase.loop();
     
     (theBase.*func)(); 
     
     myApp theDerived; 
     
     theDerived.loop();
     
     (theDerived.*func)(); 
     
     return 0; 
     
}
