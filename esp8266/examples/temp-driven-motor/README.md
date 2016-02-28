Temperature driven motor by PWM
-------------------------------

This example reads a temperature from a DS18B20 Temp-Sensor and uses a lookup
table to choose a PWM value to drive the motor at.

As the temperature detected rises or falls, different run rates can be applied
to the motor.

For example, this might be used to drive a fan where the fan speed increases
as the temperature detected by the temperature sensor increases.

The values in the lookup table - tempmappings - gives a temperature to start
the pwm value at and then a pwm value in the range of 0-1023 with 0 being off
and 1023 being running the motor at full speed.

A simple Web Interface is also provided so it's possible to access the device
by wifi to read the interface and see what it's doing.

To use this example, upload the init.lua script to the ESP8266 and reboot. 
