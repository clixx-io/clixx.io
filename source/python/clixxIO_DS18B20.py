
import os
import sys


'''
  SensorLog
  Turns on an LED on for one second, off for one second, repeatedly.

  This example code is in the public domain.
 '''

import time
import sys

sys.path.append("../src")
from ClixxIO import *

ts = None

class tempSensorDS18B20():
    """
     A class for getting the current temp of a DS18B20
    """

    def __init__(self, fileName):
        self.tempDir = '/sys/bus/w1/devices/'
        self.fileName = fileName
        self.currentTemp = -999
        self.correctionFactor = 1;
        self.enabled = True

    def read_temperature(self):
        if self.isEnabled():
            try:
                f = open(self.tempDir + self.fileName + "/w1_slave", 'r')
            except IOError as e:
                print "Error: File " + self.tempDir + self.fileName + "/w1_slave" + " does not exist.";
                return;

            lines=f.readlines()
            crcLine=lines[0]
            tempLine=lines[1]
            result_list = tempLine.split("=")

            temp = float(result_list[-1])/1000 # temp in Celcius

            temp = temp + self.correctionFactor # correction factor

            #if you want to convert to fahrenheit, uncomment this line
            #temp = (9.0/5.0)*temp + 32  

            if crcLine.find("NO") > -1:
                temp = -999

            self.currentTemp = temp
            return temp

            #print "Current: " + str(self.currentTemp) + " " + str(self.fileName)

    #setter to enable this probe
    def setEnabled(self, enabled):
        self.enabled = enabled
    #getter       
    def isEnabled(self):
        return self.enabled

# the setup routine runs once when you press reset:
def setup():
  global ts

  # initialize the digital pin as an output.
  print "Temperature Logger is starting"

  ts = tempSensorDS18B20("28-000004a2c195")

# the loop routine runs over and over again forever:
def loop():
  global ts

  sensorLog(ts.read_temperature(),'Temp2')

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

