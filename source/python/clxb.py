# /usr/bin/python

import sys
import optparse

program_input = []
callback_registrations = {}

def load_source(filepath):
    """
    Load the specified file into memory for various scans
    """
    global program_input

    program_input = [line.strip() for line in open(filepath)]
    
    print("Processing %s" % filepath)

def find_callbackregistrations():
    """
    Find all callbacks registered by the program
    """
    
    callback_list  = ["addTimerEvent",
                      "addLoopEvent",
                      "addPinChangeEvent"
                     ]
                     
    global program_input
    global callback_registrations

    for lc,l in enumerate(program_input):

        for c in callback_list:
            
            if c in l:
                print(" - Found %s on line %d" % (c,lc+1))
                callback_registrations[c] = lc

def process_file(filepath):
    
    global program_input
    
    load_source(filepath)
    
    find_callbackregistrations()
        
if __name__ == "__main__":

    print "clixx.io Event-framework program preprocessor. (c) clixx.io 2013"

    process_file(sys.argv[1])
