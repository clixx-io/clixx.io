# p2e_simple_con.py
# very simple script to make an executable file with py2exe
# put this script and your code script into the same folder
# run p2e_simple_con.py
# it will create a subfolder 'dist' where your exe file is in
# has the same name as the script_file with extension exe
# (the other subfolder 'build' can be deleted)
# note: with console code put a wait line at the end

from distutils.core import setup
import py2exe
import sys

sys.argv.append("py2exe")
sys.argv.append("-q")

# this is your code script file, change it as needed
# use it in the working directory or give full path
script_file = 'clixx'

# create a single .exe file and use compression
# (the .exe file is 30% larger with no compression)
setup(
    options = {'py2exe': {'bundle_files': 1, 'compressed': 1,}},
    # replace console with windows for a GUI program
    console = [{'script': script_file}]
)
