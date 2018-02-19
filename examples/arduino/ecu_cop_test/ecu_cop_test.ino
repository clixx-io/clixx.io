/*
Copyright 2013 David Lyon
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*/
#include <PinChangeInt.h>
 
volatile int signalFireCOP = 0;
volatile int firedCOPcount = 0;
volatile int errorCode = 0;   
volatile int errorValue = 0;

#define CYL_COUNT    4
#define CAM_TEETH_COUNT 24

#define PIN_CAMPOS 4              // the pin we are interested in
#define PIN_CNKPOS 3              // the pin we are interested in

#define ledPin 13

//COP (Coil-on-Plug) Pin numbers
int copPin[] = {3,9,7,8};

void setup() {
  
  Serial.begin(115200);
  Serial.println("Coil-on-Plug Fire program");
  
  for (int j=0; j<CYL_COUNT;j++)
  {
      pinMode(copPin[j],OUTPUT);
      digitalWrite(copPin[j],HIGH);
  }

}
 
void loop() {
  
  unsigned long currentMillis = millis();

  FirewaitingCOP();

  delay(1000);  
}

/*
Fire the next waiting Coil-on-plug
*/
void FirewaitingCOP(){

  int coptoFire = 0;
  
  // Serial.println(copPin[coptoFire], DEC);
   
  digitalWrite(copPin[coptoFire],HIGH);
  delay(4);
  digitalWrite(copPin[coptoFire],LOW);
}

