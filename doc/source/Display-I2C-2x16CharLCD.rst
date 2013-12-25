I2C 2x16 CharLCD
================

.. image:: images/CharLCD2x16.png

The Clixx.io character LCD is an easy to use display ideal
for many projects. It uses a standard Hitachi HD44780 or
compatable LCD and controlled through an MCP23003 I2C extender.

You can use this display with any Raspberry-Pi or Arduino as
well as many other different types of processor as long as they
can control an I2C bus.

Using the Display
-----------------

To use this display, it must be connected to one of the I2C
sockets on your processor Dock.

Once it's connected the backlight should come on and it will
be ready to use.

Adjusting the Backlight
-----------------------

The backlight contrast is adjustable via a small trimpot on
the front of the display. You will probably need to adjust
this to suit your needs so that the characters can be clearly
seen.

If you're using the CharLCD on the 5v Arduino, the adjustment
is going to be different than when you are running on a 3.3v
system such as the Raspberry-Pi.

This trimpot is provided so that you can adjust the display
to the voltage that you are using with your system.

Programming in Python
---------------------

There's a Python library provided to make it easy to use this
display.

.. code-block:: python

	import clixxIO_CharLCD

	if __name__ == "__main__":
	  # Set up WiringPi and connect to the IO expander
	  wiringpi2.wiringPiSetupPhys()
	  dev = wiringpi2.wiringPiI2CSetup(0x20)
	  if dev < 0:
		print "ERROR: Could not connect to device!"
		exit(1)
	  # Now create the LCD interface
	  lcd = Display(dev)
	  lcd.setup()
	  lcd.gotoXY(0, 0)
	  lcd.write("This is a sample")
	  lcd.gotoXY(7, 1)
	  lcd.write("It works!")
	  sleep(3)
	  lcd.gotoXY(0, 1)
	  lcd.write(" " * 16)
	  padding = ""
	  while True:
		lcd.gotoXY(0, 1)
		lcd.write(padding + "It works!")
		padding = padding + " "
		if len(padding) > 16:
		  padding = " "
		sleep(0.1)
    
