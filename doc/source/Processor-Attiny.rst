AVR Attiny
==========

The Digital Button is a handy general purpose button board. 

It can be used in any circuit requiring a button and can be mounted either
directly to the circuit board via a digital or analog input.

Any programming language can be used depending on your platform and the
button works as a Digital Input device meaning that a value of 0 indicates
off and a non-zero value means that the button is depressed.

You can typically add more than one Digital Button to a circuit and
combine them with other electronics such as LED's or switches to make
them do useful tasks.

.. code-block:: python

	'''
	  ButtonInput
	  Scans the input button and waits for a change, repeatedly.

	  This example code is in the public domain.
	 '''

	# Pin 2 has an Input button connected to it.
	# give it a name:
	button = 2
	button_state = False

	# the setup routine runs once when you press reset:
	def setup():
	  global button, button_state
	  
	  # initialize the digital pin as an output.
	  pinMode(button, INPUT)

	# the loop routine runs over and over again forever:
	def loop():
	  global button, button_state
	  
	  i = digitalRead(button)      # turn the LED on (HIGH is the voltage level)
	  if i != button_state:
		  print "Button is now %d" % i
		  button_state = i
		  
