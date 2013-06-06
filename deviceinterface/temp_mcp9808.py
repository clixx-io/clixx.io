

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
import sys

sys.path.append("../src")
from ClixxIO_I2C import *
from ClixxIO import *

ts = None

# the setup routine runs once when you press reset:
def setup():
  global ts

  # initialize the digital pin as an output.
  print "Temperature Logger is starting"

  ts = tempSensorMCP9808(0x18,1)

# the loop routine runs over and over again forever:
def loop():
  global ts

  sensorLog(ts.read_temperature(),'Temp1')

  sleep(1)             # wait for a second



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

