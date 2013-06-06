

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
  SensorLog
  Turns on an LED on for one second, off for one second, repeatedly.

  This example code is in the public domain.
 '''

import time

# Pin 13 has an LED connected on most Arduino boards.
# give it a name:
sensorPins = [5,2,1]

# the setup routine runs once when you press reset:
def setup():
  global sensorPins

  # initialize the digital pin as an output.
  print "Sensor Logger is starting"

  # initialize the digital pin as an output.
  for p in sensorPins:
    pinMode(p, INPUT)

# the loop routine runs over and over again forever:
def loop():
  global sensorPins

  sv = []
  for p in sensorPins:
    sv.append(digitalRead(p))

  print sv

  time.sleep(1)             # wait for a second



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

