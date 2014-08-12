#!/usr/bin/env python

"""Directory Monitor Program

This program demonstrates the inotify I/O Component ``Notify`` which can
be used for real-time monitoring of file system events. The example simply
takes a number of paths to watch as the first Command Line Argument and 
prints to stdour every file system event it sees.
"""

import sys, os
import logging

from circuits import Component
from circuits.io import Notify

processable_extensions = [".PRN",".TXT"]
watch_base_path = "/home/samba/shares/ivm_dbase/DBASE4"
watch_paths = ("COMINV","IAGINV","INSINV","SUNINV","TEMP",
               "SUNQUO","QUOPDF","LETPDF","INVPDF","PURPDF")

# create logger with 'dir-monitor'
logger = logging.getLogger('dir-monitor')
logger.setLevel(logging.DEBUG)
# create file handler which logs even debug messages
fh = logging.FileHandler('dir-monitor.log')
fh.setLevel(logging.DEBUG)
# create console handler with a higher log level
ch = logging.StreamHandler()
ch.setLevel(logging.DEBUG)
# create formatter and add it to the handlers
formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
fh.setFormatter(formatter)
ch.setFormatter(formatter)
# add the handlers to the logger
logger.addHandler(fh)
logger.addHandler(ch)

class FileWatcher(Component):

    channel = "notify"
    created_files = []

    def created(self, filename, path, fullpath, isdir):
        if not os.path.isdir(fullpath):
            self.created_files.append(fullpath)
            logger.debug("File {0:s} created".format(fullpath))

    def modified(self, filename, path, fullpath, isdir):
        if not os.path.isdir(fullpath):
            logger.debug("File {0:s} modified".format(fullpath))

    def opened(self, filename, path, fullpath, isdir):
        if not os.path.isdir(fullpath):
            logger.debug("File {0:s} opened".format(fullpath))

    def write(self, filename, path, fullpath, isdir):
        logger.debug("File {0:s} written".format(fullpath))

    def closed(self, filename, path, fullpath, isdir):
        if not os.path.isdir(fullpath):
            logger.debug("File {0:s} closed".format(filename))

            if fullpath in self.created_files:

                # Find the entry in the list and then delete it
                i = self.created_files.index(fullpath)
                del self.created_files[i]

                fext = fullpath[-4:]
                if fext in processable_extensions:
                    logger.info("File {0:s} ready for processing".format(filename))

if __name__ == "__main__":
            
    # Configure the system
    app = Notify()
    FileWatcher().register(app)

    # Add the path to watch
    for p in watch_paths:
        app.add_path(os.path.join(watch_base_path,p))

    # Run the system
    app.run()
