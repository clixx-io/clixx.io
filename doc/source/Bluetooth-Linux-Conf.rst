Bluetooth setup under Linux
===========================

.. image:: images/Button-Board.png

The Bluetooth HC-05 modules are Entry-Level Bluetooth devices that provide 
an easy way to set up a serial between an embedded device to a host or
the Internet.  


Connection
^^^^^^^^^^

This board can be plugged into any Slot capable of Serial Input/Output.

Linux Configuration
^^^^^^^^^^^^^^^^^^^

Setting up Linux to use these modules as Serial Ports is quite straightforward.

The first step is to scan to find their Address. This is done with the command:

  hcitool scan

Once you have the Bluetooth address of the module then you can copy that
to the clipboard. With your favourite editor, edit the /etc/bluetooth/rfcomm.conf
file and paste in the Module address.

Save the file and restart bluetooth with this command:

  service bluetooth restart

The next thing to do is to start the bluetooth Pin-Code Agent. This is needed
to enter the Pin Code when the module asks for it. By default all HC-05 modules 
have the pin code 1234.

  bluetooth-agent 1234 &

Everything should now be ready to connect to the modules. This is done
using the following command:

  rfcomm /dev/rfcomm0 9600

All that's needed now is to connect to the /dev/rfcomm0 device. You can
do this from a program or with a simple terminal emulator like:

  screen /dev/rfcomm0 9600



Code Example
^^^^^^^^^^^^

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
		  
Circuit
^^^^^^^

The circuit includes a Button with a 2K Resistor.

Modifications
^^^^^^^^^^^^^

This board includes a work-area at the top for soldering on extra components.
