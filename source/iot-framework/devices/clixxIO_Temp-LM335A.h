/*
  LM335A.h - Library for using a LM335A temperature sensor.
  Created by Jonathan Merrill, February 20, 2010.
    http://www.greenrobotics.net
  Released into the public domain.
*/
#ifndef LM335A_h
#define LM335A_h

class LM335A
{
  public:
    LM335A(int pin);
    void ReadTemp();
	float Fahrenheit() const;
	float Celsius() const;
	float Kelvin() const;
  private:
    int _pin;
    float _Kelvin;
	float _Celsius;
	float _Fahrenheit;
	float _sensorValue;

};

#endif

