#!/usr/bin/python

import argparse
import sys

sys.path.append("src")

from ClixxIO import *

parser = argparse.ArgumentParser(description='Command Line Device Interface.')


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

 clixx.io add button pin/gpio X
 clixx.io add led d1
 clixx.io rm Temp1
 clixx.io start Temp1
 clixx.io stop Temp1
 
"""

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
  #  parser.add_argument('--add', nargs=2)
    parser.add_argument('device', nargs='+')

    parser.add_argument("-v", "--verbose", help="increase output verbosity",
                    action="store_true")
   
    args = parser.parse_args()
    
    if args.device[0]=="list":
        print clixxIOReadDevices()
    elif args.device[0]=='history':
        print clixxIOLatestValues(args.device[1])
    elif args.device[0]=="status":
        print clixxIOReadDevice(args.device[1]) 
