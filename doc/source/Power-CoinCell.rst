Coin Cell Battery Board
-----------------------

.. image:: images/LED-Board.png

The Digital LED Board is a very simple board that's commonly used as a 
status indicator on digital devices.

It's implemented within clixx.io as a board because there are many times
when having a board can be handy. It attaches as an output device and can
be made to turn on or off according to the output pin status.

It has two states on and off and can be made to flash, turn-on 
or vary in intensity according to microprocessor control.

.. code-block:: python

	'''
	  Blink
	  Turns on an LED on for one second, off for one second, repeatedly.

	  This example code is in the public domain.
	 '''

	# Pin 13 has an LED connected on most Arduino boards.
	# give it a name:
	led = 13

	# the setup routine runs once when you press reset:
	def setup():
	  # initialize the digital pin as an output.
	  pinMode(led, OUTPUT)


	# the loop routine runs over and over again forever:
	def loop():
	  digitalWrite(led, HIGH)   # turn the LED on (HIGH is the voltage level)
	  delay(1000)               # wait for a second
	  digitalWrite(led, LOW)    # turn the LED off by making the voltage LOW
	  delay(1000)               # wait for a second

