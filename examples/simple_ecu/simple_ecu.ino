/*
Copyright 2013 David Lyon
This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
*/
#include <PinChangeInt.h>
 
volatile int camposCounter = 0;   // Basic counter for cam sensor
volatile int camCount = 0;   
volatile int rpmCount = 0;   
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
int copPin[] = {5,9,7,8};

int ledState = LOW;               // ledState used to set the LED
long previousMillis = 0;          // will store last time LED was updated
long interval = 500;             // interval at which to blink (milliseconds)

byte cmd=0;                       // a place to put our serial data
 
void setup() {
  
  Serial.begin(115200);
  Serial.println("Coil-on-Plug Fire program");
  
  for (int j=0; j<CYL_COUNT;j++)
  {
      pinMode(copPin[j],OUTPUT);
      digitalWrite(copPin[j],HIGH);
  }

  pinMode(PIN_CAMPOS, INPUT);         //set the pin to input
  pinMode(PIN_CNKPOS, INPUT);         //set the pin to input

  // digitalWrite(PIN_CAMPOS, HIGH); //uncomment to use the internal pullup resistor
  PCintPort::attachInterrupt(PIN_CAMPOS, camsensorpulse, RISING); // attach a PinChange Interrupt to our pin on the rising edge
  PCintPort::attachInterrupt(PIN_CNKPOS, cranksensorpulse, RISING); // attach a PinChange Interrupt to our pin on the rising edge
  }
 
void loop() {
  
  unsigned long currentMillis = millis();
 
  if(currentMillis - previousMillis > interval) {
    // save the last time you blinked the LED 
    previousMillis = currentMillis;   

    // if the LED is off turn it on and vice-versa:
    if (rpmCount == 0)
      ledState = LOW;
    else
      ledState = HIGH;

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
    
    if ((rpmCount == 0) && (camCount == 0))
      Serial.print(".");
    else {
      int cr = rpmCount,cm = camCount;
      camCount = rpmCount = 0;
      
      Serial.print("cam:\t");
      Serial.print(cm, DEC);
      Serial.print("\trpm:\t");
      Serial.println(cr, DEC);
    }
    
  }

  if (errorCode != 0){
    
    // If an Error Code is detected then report it
    Serial.print("error:\t");
    Serial.print(errorCode, DEC);
    Serial.print(" value:\t");
    Serial.println(errorValue, DEC);

    // Reset the Error Code    
    errorCode = 0;
    
  }

  if (signalFireCOP != 0)
  {
    FirewaitingCOP();
  }  
  
}

/*
ISR for camsensor pulses
*/
void camsensorpulse()
{
  camCount++;
  camposCounter++;
  if (camposCounter % 6 == 0)
  {
    signalFireCOP = camposCounter;
  }
}

/*
ISR for cranksensor pulses
*/
void cranksensorpulse()
{
  if (camposCounter != CAM_TEETH_COUNT)
  {
    errorCode = 1;
    errorValue = camposCounter;
    
  }
  if (firedCOPcount != CYL_COUNT)
  {
    errorCode = 2;
    errorValue = firedCOPcount;
    
  }
    
  camposCounter = 0;
  firedCOPcount = 0;

  rpmCount++;   
}

/*
Fire the next waiting Coil-on-plug
*/
void FirewaitingCOP(){

  int coptoFire = (signalFireCOP / 6) - 1;
  
  signalFireCOP = 0;

  firedCOPcount++;
  
  // Serial.println(copPin[coptoFire], DEC);
   
  digitalWrite(copPin[coptoFire],LOW);
  delay(10);
  digitalWrite(copPin[coptoFire],HIGH);
}

