#

# Pin Definitions for the RaspberryPi WiringPi2
Serial_1_i = 2
Serial_1_o = 3

Digital_1_i = 5
Digital_1_o = 6
Digital_1_s = 4

Digital_2_i = 2
Digital_2_o = 3
Digital_2_s = 0

Digital_3_i = 1
Digital_3_o = 11
Digital_3_s = 10

import os

import logging
clixxIOlogger = logging.getLogger()
clixxIOlogger.setLevel(logging.DEBUG)

formatter = logging.Formatter('%(asctime)s, %(message)s')
# Console Logging Handler
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
ch.setFormatter(formatter)
clixxIOlogger.addHandler(ch)

logginghandlers = {}

def sensorLogPath(sensorname):
    """
    Returns the full path of a Log file for a particular sensor
    """
    f = os.path.join("~/",sensorname + ".csv")
    f = os.path.join(sensorname + ".csv")
    return f

def sensorLog(value,sensorname):
    """
    Sends a Sensor Value to the logging system
    """
    print value

    if sensorname not in logginghandlers.keys():

        fh = logging.FileHandler(sensorLogPath(sensorname))

        fh.setLevel(logging.DEBUG)
        fh.setFormatter(formatter)
        clixxIOlogger.addHandler(fh)

        logginghandlers[sensorname] = fh


    clixxIOlogger.info(value)

    return

