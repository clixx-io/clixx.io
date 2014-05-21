# /usr/bin/python
"""
 * Copyright (c) 2014 clixx.io
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the Clixx.io nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CLIXX.IO BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
"""

import os, sys, mmap, ctypes, struct, json, platform
import logging, glob
from ConfigParser import ConfigParser
from datetime import datetime
from time import *

# Internationalisation
import gettext
_ = gettext.gettext


clixxIOConfigName= "clixx.IO.conf"
clixxIOConfigDirSuffix = ".local/share/clixx.io"

IoTProjectDirSuffix = "IoT"

formatter = logging.Formatter('%(asctime)s, %(message)s',"%Y-%m-%d %H:%M:%S")
# Console Logging Handler
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
ch.setFormatter(formatter)

clixxIOlogger = logging.getLogger()
clixxIOlogger.setLevel(logging.DEBUG)
clixxIOlogger.addHandler(ch)

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

logginghandlers = {}

def spawntask(cmdline):
    # Put stderr and stdout into pipes
    proc = subprocess.Popen(cmdline, \
        shell=True, stderr=subprocess.PIPE, stdout=subprocess.PIPE)
    return_code = proc.wait()

    output = []
    # Read from pipes
    for line in proc.stdout:
        output += line
    for line in proc.stderr:
        print("stderr: " + line.rstrip())

    return (return_code,output)

def GetConfigDir():
    """ 
    Return: The Location for configuration files
    """

    homedir = ''

    if platform.system()=='Windows':	
        try:
            from win32com.shell import shellcon, shell            
            homedir = shell.SHGetFolderPath(0, shellcon.CSIDL_APPDATA, 0, 0)
 
        except ImportError: # quick semi-nasty fallback for non-windows/win32com case
            homedir = os.path.expanduser("~")
            homedir = os.path.join(homedir,"clixx.io")

        else:
            homedir = os.path.join(os.path.expanduser("~"),clixxIOConfigDir)

    elif platform.system()=='Linux':	

        clixxIOLogDir    = "/var/log"

        homedir = os.path.join(os.path.expanduser("~"),clixxIOConfigDirSuffix)

    return homedir 

def sensorLogPath(sensorname):
    """
    Returns the full path of a Log file for a particular sensor
    """
    f = os.path.join(GetConfigDir(),SensorLogDir,sensorname + ".csv")

    return f

class ow_system:
    """ 
    Provided to extend access to the Dallas One Communication Bus

    For Ubuntu, here's a list of packages: https://launchpad.net/ubuntu/+source/owfs
    """

    def linux_install(self):

        spawntask("apt-get install -y owfs ow-shell python-ow")

class i2c_system:
    """ 
    Provided to extend access to the I2C Bus.
    """

    def __init__(self, busnumber = 1):

        self.busnumber = busnumber

        try:
            import smbus
        except ImportError:
            sys.stderr.write("Warning: smbus module is not installed...")

    def rpi_install(self):
      """
      Function to install the necessary software to run I2C

      Based on http://www.instructables.com/id/Raspberry-Pi-I2C-Python/step2/Enable-I2C/

      """

      # Update module blacklist file
      blacklist_conf = open("/etc/modprobe.d/raspi-blacklist.conf")
      lines = blacklist_conf.readlines()
      blacklist_conf.close()
      new_content = []
      for l in lines:
          if l.rstrip() == 'blacklist i2c-bcm2708':
              new_content.append('#blacklist i2c-bcm2708\n')
          else:
              new_content.append(l.rstrip())

      blacklist_conf = open("/etc/modprobe.d/raspi-blacklist.conf",'w')
      blacklist_conf.write('\n'.join(new_content))

      # Update /etc/modules
      etc_modules = open("/etc/modules")
      lines = etc_modules.readlines()
      etc_modules.close()
      new_content = []
      i2c_found = False
      for l in lines:
          if 'i2c-dev' in l:
              i2c_found = True
      if not i2c_found:
          etc_modules = open("/etc/modules",'w+')
          etc_modules.write(''.join(lines)+'\ni2c-dev\n')
          etc_modules.close()

      # http://www.instructables.com/id/Raspberry-Pi-I2C-Python/step4/Install-Necessary-Packages/
      if not os.path.exists("/usr/sbin/i2cdetect"):
        spawntask("apt-get install -y i2c-tools")
        
      spawntask("apt-get install -y python-smbus")
      
      spawntask("adduser pi i2c")

    def installed(self):        
    
      return os.path.exists("/usr/sbin/i2cdetect")

    def scan(self):
       """ P
       erform a scan on the I2C Bus
       """
       devices = []
       rcode,o = spawntask("i2cdetect -y %d" % self.busnumber)
       if (rcode == 0):
         for l in o:
            r = s[3:].replace(' --','')
       return devices

    def probe(self):    
      return

# General i2c device class so that other devices can be added easily
class i2c_device:
   
    def __init__(self, addr, port):
        
        try:
            import smbus
        except ImportError:
            sys.stderr.write("Warning: smbus module is not installed...")

        self.addr = addr
        self.bus = smbus.SMBus(port)

    def write(self, byte):
        self.bus.write_byte(self.addr, byte)

    def read(self):
        return self.bus.read_byte(self.addr)

    def read_nbytes_data(self, data, n): # For sequential reads > 1 byte
        return self.bus.read_i2c_block_data(self.addr, data, n)

class mcp23017:
    """
    http://hertaville.com/2013/04/01/interfacing-an-i2c-gpio-expander-mcp23017-to-the-raspberry-pi-using-c/
    """

class displayRotaryLeds(i2c_device):
    """ 
    Rotary LED display based on the PCF8574
    """
    def setbits(self,value):
        self.write( 0xff ^ value)
        return

    def display(self, value, maximum):
        v = (value * 8) / maximum
        if v < 1:
            self.setbits(0x00)
        if v < 2:
            self.setbits(0x01)
        elif v < 3:
            self.setbits(0x03)
        elif v < 4:
            self.setbits(0x07)
        elif v < 5:
            self.setbits(0x0f)
        elif v < 6:
            self.setbits(0x1f)
        elif v < 7:
            self.setbits(0x3f)
        elif v < 8:
            self.setbits(0x7f)
        elif v < 9:
            self.setbits(0xff)

class tempSensorMCP9808(i2c_device):
    """ 
    MCP9808 is an I2C Temperature sensor
    """
    def read_temperature(self):
        b = self.read_nbytes_data(5,3)
        t = (b[1] / 16) + b[2] * 16
        return t

class RaspberryPiPinOuts:
    # Pin Definitions for the RaspberryPi WiringPi2
    
    pins = {'Serial_1' : {'i':2, 'o':3},
            'Digital_1' : {'i':5, 'o':6, '*':4},
            'Digital_2' : {'i':2, 'o':3, '*':0},
            'Digital_3' : {'i':1, 'o':11, '*':10}
           }

    def pin(portname,portpin):
        return pins[portname,portpin]

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
    """
    Sensor Data is exchanged using Shared Memory on Linux. This
    method configures the shared memory space so that multiple
    clients can read/write to the space.
    """
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
    """
    Low Level routine to write to the shared-memory-value area
    """
    # Now ceate a pointer to the shared memory area
    s_type = ctypes.c_char * len(value)
 
    # Now create the ctypes instance
    s = s_type.from_buffer(clixxIOshmBuff, 0)
    # And finally set it
    s.raw = value
 
    os.lseek(clixxIOshmfd,0,os.SEEK_SET)
    os.write(clixxIOshmfd,value + '\n')

    return

def clixxIOReadSHM():
    """
    Low Level routine to read from the shared-memory-value area
    """

    # global clixxIOshmfd, clixxIOshmBuff

    if clixxIOshmfd == None:
        clixxIOSetupSHM()

    os.lseek(clixxIOshmfd,0,os.SEEK_SET)
    s = os.read(clixxIOshmfd,mmap.PAGESIZE)

    # And finally set it
    return s[:s.index("\n")]

def clixxIOReadDevice(deviceID):
    """
    Low Level routine to read values for a particular device
    """

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
    """
    Low Level routine to read values for all devices
    """

    clixxIOConfig.read(os.path.join(clixxIOConfigDir,clixxIOConfigName))

    alldevices = []

    for k in clixxIOConfig._sections.keys():
        if k.startswith('Device-'):
            alldevices.append(k[7:])

    return alldevices

def clixxIOUpdateDevice(deviceInfo):
    """
    Low Level routine to update the Share-Memory-Space for one device
    """

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
    """
    Read the latest values for a particular device from the device log file
    """

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
    """
    Read known summary metadata for a device
    """

    return results

def clixxIOAddProject(projectname):
    return

def clixxIOAddProjectDevice(projectname):
    return

def clixxIOStartProject(projectname):
    return

def clixxIOListProjects():
    """
    Return the names of all projects maintained by the system.
	
    These are typically directories stored in the IoT directory
    """
    IoTdir = os.path.join(os.path.expanduser("~"),IoTProjectDirSuffix,'*')

    projects = []
    projectdirs = glob.glob(IoTdir)
    for d in projectdirs:
        if os.path.basename(d) != 'libraries':
            projects.append(os.path.basename(d))
    return projects

def clixxIOStopProject(projectname):
    return
