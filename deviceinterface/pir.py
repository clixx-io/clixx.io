#/usr/bin/python

'''
  PIR Sensor
  Scans the PIR sensor and waits for a change, repeatedly.

  This example code is in the public domain.
 '''


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

sys.path.append("../src")
from ClixxIO import *

# Pin 5 has a PIR sensor connected to it.
pir_sensor = 5 # sensorPin("Pir1")
pir_state = False

# the setup routine runs once when you press reset:
def setup():
  global pir_sensor, pir_state
  
  # initialize the digital pin as an output.
  pinMode(pir_sensor, INPUT)

# the loop routine runs over and over again forever:
def loop():
  global pir_sensor, pir_state

  i = digitalRead(pir_sensor)      # turn the LED on (HIGH is the voltage level)
  if i != pir_state:
      # print "Pir is now %d" % i
      sensorLog(pir_state,"Pir1")
      sensorLog(i,"Pir1")
      pir_state = i


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

