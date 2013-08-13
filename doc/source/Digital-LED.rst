Digital LED Baord
-----------------

The Digital LED Board is one of the simplest but sometimes most
entertaining board of the set. It has two states on and off and
can be made to flash, turn-on or vary in intensity according to
microprocessor control.

<photo>

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

