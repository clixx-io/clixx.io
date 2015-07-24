Terminal-ESP8266
================

Terminal-ESP8266 is a very simple terminal program designed
for the ESP8266 and NodeMCU firmware to make it easy for
beginners to get started.

It provides some automated abilities to allow you to connect
to the device via the USB and talk to it via the simple serial
port interface.

There are more advanced IDE's available such as EspLorer but
we just provide the basics in one program to make it all easy.

Shown here is the main window:

Connecting to the USB-Serial
----------------------------

First you need to open up a Serial connection with the device
over the serial port. To do this you need to open a serial
port.

On Windows this will normally be COM5: and and Linux it's
normally /dev/ttyUSB0.

Press the 'Connect' button

First steps - talking to the ESP8266
------------------------------------

Once you have the device open you can now start talking to
it via the console window.

Simply type a command and press the 'Send' button.

    print("Hello World!")
    
Setting up Wifi - Scan for Access Points
----------------------------------------

Since the ESP8266 has native wifi, the first step is to
connect it to your access point.

To do this press the 'Scan' button.

Once this is complete a list of all the wifi Access points
that are visible will be shown.

Setting up Wifi - Connect to your Access Point
----------------------------------------------

Select or type in your SSID and enter your wifi password.

Press [Connect]

Display your IP address
-----------------------

The final step is to display for IP address. Click the
[Display IP] button.

Sending other Commands
----------------------

The Terminal-ESP8266 program allows you to interact with
your device and you can send it whatever commands you need
via the command window.
