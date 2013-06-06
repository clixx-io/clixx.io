

import os
import sys
if os.path.exists("WiringPy"):
    sys.path.append("WiringPy")
if os.path.exists("../../WiringPy"):
    sys.path.append("../../WiringPy/WiringPy")
if os.path.exists("../../../WiringPy"):
    sys.path.append("../../../WiringPy/WiringPy")

try:
    from wiringpi2 import *
    from libwiringpi2 import *
except ImportError:
    from librpigpio import *

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
      


if __name__ == "__main__":

    setup()

    try:
        while (1):
            loop()
    except (KeyboardInterrupt, SystemExit):
        pass
    except:
        # report error and proceed
        raise

