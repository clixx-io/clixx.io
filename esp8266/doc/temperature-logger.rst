Temperature Logger Project:
===========================

This project uses LM75 digital temperature sensor connected to esp8266 module on i2c interface
and logs the sensor data to the webserver after an interval of every 30 seconds.

1) Connect the system to host machine (linux) like this

host machine's usb port <--> USB-TTL <--> esp8266 module <--> LM75

2) Create a new IoT project with the name templog

clixx.io$ source/tools/clixx new project templog

3) Run the webserver on linux host machine (replace 0.0.0.0 in webinterface.py with the IP address of host machine running webserver) 

clixx.io$ python source/webinterface/webinterface.py
 
4) Open the terminal program and setup wifi by providing the SSID and Password and hiting "Setup wifi" button 

clixx.io$ sudo python esp8266/tools/terminal-esp8266/terminal-esp8266.py 

5) Modify the ./examples_esp8266/temp-logger-csv/init.lua script with IP address of the host machine running webserver

sk:connect(5000,"192.168.43.94")

6) Copy the lua scripts to esp module 

clixx.io$ sudo python ../esptools/esptools/espcp.py -f ./examples_esp8266/temp-logger-csv/init.lua
clixx.io$ sudo python ../esptools/esptools/espcp.py -f ./esp8266/source/lm75.lua

7) Hit the restart button on the terminal program

8) esp module will now restart and start logging the temperature values to webserver which can be seen anywhere on the network using web browser and browsing the frank project

http://192.168.43.94:5000/ 

where 192.168.43.94 is IP address of webserver

