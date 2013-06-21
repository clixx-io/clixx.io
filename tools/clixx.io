#!/usr/bin/python

import sys
sys.path.append("src")

from ClixxIO import *

useage = """
clixx.io Command Line
---------------------

Enhance clixx.io with command line interface to monitor
sensors. 

 clixx.io --latest log Temp1      # Display the log
 clixx.io info Temp1              # Info about the sensor
 cixx.io disconnect Temp1         # Safely disconnect the device
 clixx.io connect Temp1

 clixx.io list                    # List all devices
 
 clixx.io brandname "Scrooge Engineering"
 
 clixx.io webserver start
 clixx.io webserver stop

 clixx.io jabber start
 clixx.io jabber stop

 clixx.io server start            # TcpIP server 
 clixx.io server stop

"""

if __name__ == "__main__":

    if len(sys.argv) == 0:
        print useage
        sys.exit(0)
		
    if sys.argv[1]=="list":
        print clixxIOReadDevices()
 
