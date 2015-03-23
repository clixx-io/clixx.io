#!/usr/bin/env python

"""Directory Watch Example

This program demonstrates the inotify I/O Component ``Notify`` which can
be used for real-time monitoring of file system events. The example simply
takes a number of paths to watch as the first Command Line Argument and 
prints to stdour every file system event it sees.
"""

import sys, os

from circuits import Component
from circuits.io import Notify

processable_extensions = [".PRN",".TXT"]
watch_base_path = "~/"
watch_paths = ("IoT")

class FileWatcher(Component):

    channel = "notify"
    created_files = []

    def created(self, filename, path, fullpath, isdir):
        if not os.path.isdir(fullpath):
            self.created_files.append(fullpath)
            print("File {0:s} created".format(fullpath))

    def modified(self, filename, path, fullpath, isdir):
        if not os.path.isdir(fullpath):
            print("File {0:s} modified".format(fullpath))

    def opened(self, filename, path, fullpath, isdir):
        if not os.path.isdir(fullpath):
            print("File {0:s} opened".format(fullpath))

    def write(self, filename, path, fullpath, isdir):
        print("File {0:s} written".format(fullpath))

    def closed(self, filename, path, fullpath, isdir):
        if not os.path.isdir(fullpath):
            print("File {0:s} closed".format(filename))

            if fullpath in self.created_files:

                # Find the entry in the list and then delete it
                i = self.created_files.index(fullpath)
                del self.created_files[i]

                fext = fullpath[-4:]
                if fext in processable_extensions:
                    print("File {0:s} ready for processing".format(filename))

if __name__ == "__main__":
            
    # Configure the system
    app = Notify()
    FileWatcher().register(app)

    # Add the path to watch
    for p in watch_paths:
        app.add_path(os.path.join(watch_base_path,p))

    # Run the system
    app.run()
