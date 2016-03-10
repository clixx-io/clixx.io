Simple Web Driven PWM
---------------------

This example allows you to control a Device via a GPIO port
driven by Pulse-Width-Modululation over the Web.

To use this example, upload the project to your device then
access it via your web browser.

![Screenshot of Webpage](https://github.com/clixx-io/clixx.io/blob/master/doc/source/images/screenshot-web-pwm.PNG)

### How this can be used ##

This example can be used to variably control lighting or DC-Motors remotely.

### Controlling the Device from the Command-Line over the Network ##

You can use the popular command wget to control the device over the command line:

To Turn the motor on:

    wget http://192.168.0.15/?pwmvalue=1023&pin=ON
   
To Turn the motor off:
    
    wget http://192.168.0.15/?pwmvalue=0&pin=OFF
    
To run the motor at half power:
    
    wget http://192.168.0.15/?pwmvalue=512&pin=ON

