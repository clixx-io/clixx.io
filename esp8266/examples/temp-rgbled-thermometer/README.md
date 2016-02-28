Temperature driven RGB-LED
--------------------------

This example reads a temperature from a DS18B20 Temp-Sensor and then displays
a "feels-like" value to an RGB-LED.

At -3 degrees or less, the LED will start off Blue, then as the temperature
increases, different temperatures will get displayed according to pre-chosen
colours in a table of constants in the program. At over 35-degrees, an Orangy
Red colour will be visible.

To change the colours being displayed, it's best to change the indexes into
the list of colours, rather than the master-colour list (colorvals).

To use this example, upload the init.lua script to the ESP8266 and reboot. 
