#!/usr/bin/python

import argparse
import sys
sys.path.append("src")

from ClixxIO import *

parser = argparse.ArgumentParser(description='Command Line Device Interface.')
#parser.add_argument('integers', metavar='N', type=int, nargs='+',
#                   help='an integer for the accumulator')
#parser.add_argument('--sum', dest='accumulate', action='store_const',
#                   const=sum, default=max,
#                   help='sum the integers (default: find the max)')
#
#args = parser.parse_args()
#print(args.accumulate(args.integers))


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

 clixx.io add button d1
 clixx.io add led d1
 clixx.io rm L
"""

if __name__ == "__main__":

    if len(sys.argv) == 0:
        print useage
        sys.exit(0)

    if sys.argv[1]=="list":
        print clixxIOReadDevices()
    elif sys.argv[1]=='history':
        print clixxIOLatestValues(sys.argv[2])
    elif sys.argv[2]=="status":
        print clixxIOReadDevice(sys.argv[2]) 
