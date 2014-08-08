/*
  LM335A.h - Library for LM335A temperature sensor
  Copyright (c) 2010 Jonathan Merrill.  All right reserved.
  http://www.greenrobotics.net
*/
#include "LM335A.h"

LM335A::LM335A(int pin)
{
  pinMode(pin, INPUT);
  _pin = pin;
}

void LM335A::ReadTemp()
{
  int KelvinC=273;
  _sensorValue = analogRead(_pin);
  _Kelvin = (((_sensorValue / 1023) * 5) * 100); // convert 
  _Celsius=_Kelvin-KelvinC;
  _Fahrenheit=(_Celsius)*(9/5)+32;
}

float LM335A::Fahrenheit() const
{
	return(_Fahrenheit);
}

float LM335A::Celsius() const
{
	return(_Celsius);
}

float LM335A::Kelvin() const
{
	return(_Kelvin);
}
