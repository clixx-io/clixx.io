#!/usr/bin/python
# -*- coding: utf-8 -*-

# portable serial port access with python
# this is a wrapper module for different platform implementations of the
# port enumeration feature
#
# (C) 2011 Chris Liechti <cliechti@gmx.net>
# this is distributed under a free software license, see license.txt

"""\
This module will provide a function called comports that returns an
iterable (generator or list) that will enumerate available com ports. Note that
on some systems non-existent ports may be listed.

Additionally a grep function is supplied that can be used to search for ports
based on their descriptions or hardware ID.
"""

import sys, os, re

# chose an implementation, depending on os
#~ if sys.platform == 'cli':
#~ else:
import os
# chose an implementation, depending on os
if os.name == 'nt': #sys.platform == 'win32':
    from serial.tools.list_ports_windows import *
elif os.name == 'posix':
    from serial.tools.list_ports_posix import *
#~ elif os.name == 'java':
else:
    raise ImportError("Sorry: no implementation for your platform ('%s') available" % (os.name,))


class PortsListener:
	"""\
	List serial ports with optional regexp filtering.
	"""
	
	def getPorts(regexp = None):
		hits = 0
	    # Get iteraror w/ or w/o filter
		if regexp:
		    #print("Filtered list with regexp: %r" % (regexp,))
		    iterator = sorted(grep(regexp))
		else:
		    iterator = sorted(comports())
		    
		# List them
		ports = []
		for port, desc, hwid in iterator:
		    ports.append(port)

		    hits += 1
		    
		return ports


def grep(regexp):
    """\
    Search for ports using a regular expression. Port name, description and
    hardware ID are searched. The function returns an iterable that returns the
    same tuples as comport() would do.
    """
    for port, desc, hwid in comports():
        if re.search(regexp, port, re.I) or re.search(regexp, desc) or re.search(regexp, hwid):
            yield port, desc, hwid


def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='PortsListener - list for serial ports')
    
    parser.add_argument('-r', '--regexp',
	                    help='Filter list with regexp',
	                    nargs=1)

    p = parser.parse_args()

    print(PortsListener.getPorts(p.regexp))

# test
if __name__ == '__main__':
    main()
