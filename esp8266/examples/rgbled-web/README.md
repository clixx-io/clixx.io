Simple Web Driven RGB-LED
---------------------

This example allows you to control a WS2812 RGB-LED via a simple web interface.

To use this example, upload the project to your device then
access it via your web browser.

![Screenshot of Webpage](https://github.com/clixx-io/clixx.io/blob/master/doc/source/images/screenshot-web-rgbled.PNG)

### How this can be used ##

This example can be used to variably control one RGB-LED or modified to control more.

### Controlling the Device from the Command-Line over the Network ##

You can use the popular command wget to control the device over the command line:

To Turn the LED on:

    wget http://192.168.0.13/?r=50&g=0&b=20&pin=ON
   
To Turn the RGB-LED off:
    
    wget http://192.168.0.13/?pin=OFF
    
