The motivation behind this is that the application gives the User a place to manage their collection of IoT devices: sprinkler, solar-charges whatever from the system try in windows.

Installation
------------

1) First install python2.7 but not 3.0 as well as pyside
2) At the moment, change directory to clixx.io
2) run "python setup.py install" which will go install everything
2) change directory to qt-gui
3) run the command "python iot-controller.py"

How it works
------------

There should be a directory created /Users/xx/IoT and under this directory are created all your 'IoT' projects.

Each .ini file specifies what 'commands' are placed on the menu. They're just command lines to send commands via anything to any device.

The .ini file that looks like this:

-------------------------------------------------------------------------
#
#   A simple configuration file created by the clixx.io generator for
#   holding configuration information.
#

[munly5]
project_type = c++

[mqtt]
output_channel = ['/fridgecommands']
input_channel = ['/fridgecommands']
commands = [[]]
mqtt_pub = mosquitto_pub

[menu_actions]
Make = make
Deploy = make deploy
Clean = make clean
Log = http://localhost:9001/log
Edit = http://localhost:9001/programedit
Open = http://www.google.com
On = mosquitto_pub On
Off = mosquitto_pub Off

[events]
program_setup = true
program_timers = true
serial_line = true

[devices]
list =
    switch
    button
    dial

[connections-linux]
switch=
button=
dial=

[connections-avr]
switch=
button=
dial=
