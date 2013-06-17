#

import os
import logging
import sys
from ConfigParser import ConfigParser

# Internationalisation
import gettext
_ = gettext.gettext

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

SensorLogDir = "My Sensor Logs"

clixxIOlogger = logging.getLogger()
clixxIOlogger.setLevel(logging.DEBUG)

clixxIODevices = {}
clixxIODeviceKeys = ["status","value","recentHistory","lastActive","driverName",
                     "sensorId","sensorDescription","logDateTime","logFileSize"]
clixxIODeviceStatus = {"connected":_("Connected"),"not_connected":_("Not Connected"),"running":_("Running")}

clixxIOBranding = {}
clixxIOBrandingSection = "System"
clixxIOBrandingKeys = ["systemname","ownername","providername","provideraddress","copyrightmsg"]

clixxIOConfig = ConfigParser()

formatter = logging.Formatter('%(asctime)s, %(message)s',"%Y-%m-%d %H:%M:%S")
# Console Logging Handler
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
ch.setFormatter(formatter)
clixxIOlogger.addHandler(ch)

logginghandlers = {}

if sys.platform == "linux2":
    clixxIOLogDir    = "/var/log"
    clixxIOConfigDir = "/etc"
clixxIOConfigDir = os.path.join(os.getenv("HOME"),"clixx.io")

clixxIOConfigName= "clixx.io.conf"
SensorLogDir     = "My Sensor Logs"

def sensorLogPath(sensorname):
    """
    Returns the full path of a Log file for a particular sensor
    """
    f = os.path.join(os.getenv("HOME"),SensorLogDir,sensorname + ".csv")
    f = os.path.join("../",SensorLogDir,sensorname + ".csv")

    return f

def sensorLog(value,sensorname):
    """
    Sends a Sensor Value to the logging system
    """
    if sensorname not in logginghandlers.keys():

        fh = logging.FileHandler(sensorLogPath(sensorname))

        fh.setLevel(logging.DEBUG)
        fh.setFormatter(formatter)
        clixxIOlogger.addHandler(fh)

        logginghandlers[sensorname] = fh

    if sensorname not in clixxIODevices.keys():
        
        device = {}
        
        clixxIOHistoryFill(sensorname,device)
        device["status"] = clixxIODeviceStatus["running"]
        
        clixxIODevices[sensorname] = device

    else:
        
        clixxIODevices[sensorname]["status"] = clixxIODeviceStatus["running"]
        clixxIODevices[sensorname]["value"]  = value
        
    clixxIOlogger.info(value)

    return

def configPath():
    """
    Provides the location of the configuration file
    """
    return os.path.join(clixxIOConfigDir,clixxIOConfigName)

def sensorPin(pinnumber):
    """
    Reads a sensor Pin number from the configuration file
    """
    return None

def clixxIOHistoryFillAll(sensorname):
    return

def clixxIOHistoryFill(sensorname,device):
    """
    Fills in the details of a device from configuration/log files
    """
    global clixxIOConfig
    
    clixxIOConfig.read(os.path.join(clixxIOConfigDir,clixxIOConfigName))

    for k in clixxIODeviceKeys:
        device[k] = None

    device["status"] = clixxIODeviceStatus["not_connected"]

    section_name = 'Device-' + sensorname
    print clixxIOConfig._sections.keys()

    if section_name in clixxIOConfig._sections.keys():
		if "sensordescription" in clixxIOConfig._sections[section_name].keys():
			device["sensorDescription"] = "Ambient Temperature"
    else:
        device["sensorDescription"] = "Ambient Temperature"
    return

def clixxIODeviceUpdate(sensorname,device):
    """
    Updates the information in the memory buffer
    """
    return
    
def clixxIOLoadBrandingData(pageholder):
    """
    Loads all local branding information.
    """

    clixxIOConfig.read(os.path.join(clixxIOConfigDir,clixxIOConfigName))

    for k in clixxIOBrandingKeys:
        clixxIOBranding[k] = clixxIOConfig._sections[clixxIOBrandingSection][k]
    return


