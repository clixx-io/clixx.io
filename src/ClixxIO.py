#

import os, sys, mmap, ctypes, struct, json
import logging
from ConfigParser import ConfigParser
from datetime import datetime

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

clixxIOshmPath = '/tmp/clixx.io.SHM'
clixxIOshmfd = None         # Shared Memory File Descriptor used by mmap
clixxIOshmBuff = None       # Shared Memory Handle

clixxIODevices = {}
clixxIODeviceKeys = ["status","value","recentHistory","lastActive","driverName",
                     "deviceId","sensorDescription","logDateTime","logFileSize"]
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
    f = os.path.join(os.getenv("HOME"),"clixx.io",SensorLogDir,sensorname + ".csv")
#   f = os.path.join("../",SensorLogDir,sensorname + ".csv")

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

        clixxIODevices[sensorname] = clixxIOReadDevice(sensorname)

    if value != clixxIODevices[sensorname]["value"]:

        if clixxIODevices[sensorname]["value"] is not None:
            clixxIOlogger.info(clixxIODevices[sensorname]["value"])
        clixxIOlogger.info(value)

        clixxIODevices[sensorname]["value"] = value

    clixxIODevices[sensorname]["lastActive"] = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    clixxIOUpdateDevice(clixxIODevices[sensorname])

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

def clixxIOHistoryFillAll():
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

def clixxIOSetupSHM():

    global clixxIOshmfd, clixxIOshmBuff

    if clixxIOshmfd == None:

        # Create new empty file to back memory map on disk
        if not os.path.exists(clixxIOshmPath):
            clixxIOshmfd = os.open(clixxIOshmPath, os.O_CREAT | os.O_TRUNC | os.O_RDWR)

            # Zero out the file to insure it's the right size
            os.write(clixxIOshmfd, ' ' * mmap.PAGESIZE)
            os.lseek(clixxIOshmfd,0,os.SEEK_SET)
            os.write(clixxIOshmfd, '\n')
 
        else:
            clixxIOshmfd = os.open(clixxIOshmPath, os.O_RDWR)
 
        # Create the mmap instace with the following params:
        # fd: File descriptor which backs the mapping or -1 for anonymous mapping
        # length: Must in multiples of PAGESIZE (usually 4 KB)
        # flags: MAP_SHARED means other processes can share this mmap
        # prot: PROT_WRITE means this process can write to this mmap
        clixxIOshmBuff = mmap.mmap(clixxIOshmfd, mmap.PAGESIZE, mmap.MAP_SHARED, mmap.PROT_WRITE)

def clixxIOWriteSHM(value):
    # Now ceate a string containing 'foo' by first creating a c_char array
    s_type = ctypes.c_char * len(value)
 
    # Now create the ctypes instance
    s = s_type.from_buffer(clixxIOshmBuff, 0)
 
    # And finally set it
    s.raw = value
    # Zero out the file to insure it's the right size

    # global clixxIOshmfd, clixxIOshmBuff

    os.lseek(clixxIOshmfd,0,os.SEEK_SET)
    os.write(clixxIOshmfd,value + '\n')

    return

def clixxIOReadSHM():

    # global clixxIOshmfd, clixxIOshmBuff

    if clixxIOshmfd == None:
        clixxIOSetupSHM()

    os.lseek(clixxIOshmfd,0,os.SEEK_SET)
    s = os.read(clixxIOshmfd,mmap.PAGESIZE)

    # And finally set it
    return s[:s.index("\n")]

def clixxIOReadDevice(deviceID):

    if clixxIOshmfd == None:
        clixxIOSetupSHM()

    alldevices = {}
    js = clixxIOReadSHM()
    if len(js)>0:
        alldevices = json.loads(js)

    if deviceID not in alldevices.keys():

        # Return an initialised device
        device = {}
        for k in clixxIODeviceKeys:
            device[k] = None

        device["deviceId"] = deviceID
        return device

    else:

        return alldevices[deviceID]

def clixxIOReadDevices():

    clixxIOConfig.read(os.path.join(clixxIOConfigDir,clixxIOConfigName))

    alldevices = []

    for k in clixxIOConfig._sections.keys():
        if k.startswith('Device-'):
            alldevices.append(k[7:])

    return alldevices

def clixxIOUpdateDevice(deviceInfo):

    if clixxIOshmfd == None:
        clixxIOSetupSHM()

    alldevices = {}
    js = clixxIOReadSHM()
    if len(js)>0:
        alldevices = json.loads(js)

    alldevices[deviceInfo["deviceId"]] = deviceInfo

    clixxIOWriteSHM(json.dumps(alldevices))
    
    return

def clixxIOLatestValues(deviceId,when='today'):
    

    today = datetime.now().strftime('%Y-%m-%d')

    lf = open(sensorLogPath(deviceId))

    results = []

    for l in lf.readlines():
        if l.strip().startswith(today):
            results.append(l.rstrip())

    # Use this to convert back to datetime
    entry_date = datetime.strptime('2013-05-23 10:48:46','%Y-%m-%j %H:%M:%S')

    return results

def clixxIOInfo(deviceId):


    return results
