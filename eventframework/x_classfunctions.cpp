 #include <iostream> 
 class Base { 
     public: 
     virtual void WhoAmI() const; 
     typedef void (Base::*WhoPtr)() const; 
     
}; 

class Derived: public Base { 
    public: virtual void WhoAmI() const; }; void Base::WhoAmI() 
 const { std::cout << "I am the Base" << std::endl; 
 
} 

void Derived:: WhoAmI() const 
{ 
    std::cout << "I am the Derived" << std::endl; 
}
 
 int main( int argc, char **argv ) {
     
     Base::WhoPtr func = &Base::WhoAmI; 
     
     Base theBase; (theBase.*func)(); 
     
     Derived theDerived; (theDerived.*func)(); return 0; 
     
}
