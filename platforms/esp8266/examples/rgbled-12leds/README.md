Control Multiple RGB-LEDs
-------------------------

This example shows that it's pretty straightforward to control multiple 
WS2812 RGB-LEDs with an ESP8288. All you do is plug multiple RGB-LED boards
in and then send the appropriate LUA command and you will have colours.

Here we use a strip of twelve boards connected end to end and send the LUA
commands on startup to activate them into a strip.

To use this example, upload the init.lua script to the ESP8266 and reboot. 
