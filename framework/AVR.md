Installing for AVR
------------------

For compiling avr programs we need a compiler of course. It's called avr-gcc (but the package is called gcc-avr). 

The newer version compiles code with better optimization which might be important for bootloaders which have to fit into a specific size of flash. Since 4.8.1 is not in the current stable repositories you have to do some extra steps in order to install 4.8.1 for raspberry pi. For Ubuntu just use the first line in the instructions to get 4.8.1 instead of 4.7.2. If you are a lazy guy you can start with 4.7.2 and update the packages later again.

# reload repositories and check for the gcc-avr package
sudo apt-get update
sudo apt-cache search gcc-avr

# install avr-gcc 4.7.2
sudo apt-get install gcc-avr avr-libc

# check installed version of avr-gcc 
avr-gcc -v
