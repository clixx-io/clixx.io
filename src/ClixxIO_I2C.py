
# Based on : http://www.rpiblog.com/2012/07/interfacing-16x2-lcd-with-raspberry-pi.html

try:
    import os
    import sys
    import subprocess
    import smbus
except ImportError:
    sys.stderr.write("Warning: smbus module is not installed...")
from time import *

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

# General i2c device class so that other devices can be added easily
class i2c_device:
   
 def __init__(self, addr, port):
  self.addr = addr
  self.bus = smbus.SMBus(port)

 def write(self, byte):
  self.bus.write_byte(self.addr, byte)

 def read(self):
  return self.bus.read_byte(self.addr)

 def read_nbytes_data(self, data, n): # For sequential reads > 1 byte
  return self.bus.read_i2c_block_data(self.addr, data, n)

class i2c_system:

    def install(self):
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

    def status(self):
      if os.path.exists("/usr/sbin/i2cdetect"):
        return("Installed")
      else:
        return("Not Installed")

    def scan(self):
       """ P
       erform a scan on the I2C Bus
       """
       devices = []
       rcode,o = spawntask("i2cdetect -y 1")
       if (rcode == 0):
         for l in o:
            r = s[3:].replace(' --','')
       return devices

    def probe(self):    
      return

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


class lcd2:
 #initializes objects and lcd
 '''
 Reverse Codes:
 0: lower 4 bits of expander are commands bits
 1: top 4 bits of expander are commands bits AND P0-4 P1-5 P2-6
 2: top 4 bits of expander are commands bits AND P0-6 P1-5 P2-4
 '''
 def __init__(self, addr, port, reverse=0):
  self.reverse = reverse
  self.lcd_device = i2c_device(addr, port)
  if self.reverse:
   self.lcd_device.write(0x30)
   self.lcd_strobe()
   sleep(0.0005)
   self.lcd_strobe()
   sleep(0.0005)
   self.lcd_strobe()
   sleep(0.0005)
   self.lcd_device.write(0x20)
   self.lcd_strobe()
   sleep(0.0005)
  else:
   self.lcd_device.write(0x03)
   self.lcd_strobe()
   sleep(0.0005)
   self.lcd_strobe()
   sleep(0.0005)
   self.lcd_strobe()
   sleep(0.0005)
   self.lcd_device.write(0x02)
   self.lcd_strobe()
   sleep(0.0005)

  self.lcd_write(0x28)
  self.lcd_write(0x08)
  self.lcd_write(0x01)
  self.lcd_write(0x06)
  self.lcd_write(0x0C)
  self.lcd_write(0x0F)

 # clocks EN to latch command
 def lcd_strobe(self):
  if self.reverse == 1:
   self.lcd_device.write((self.lcd_device.read() | 0x04))
   self.lcd_device.write((self.lcd_device.read() & 0xFB))
  if self.reverse == 2:
   self.lcd_device.write((self.lcd_device.read() | 0x01))
   self.lcd_device.write((self.lcd_device.read() & 0xFE))
  else:
   self.lcd_device.write((self.lcd_device.read() | 0x10))
   self.lcd_device.write((self.lcd_device.read() & 0xEF))

 # write a command to lcd
 def lcd_write(self, cmd):
  if self.reverse:
   self.lcd_device.write((cmd >> 4)<<4)
   self.lcd_strobe()
   self.lcd_device.write((cmd & 0x0F)<<4)
   self.lcd_strobe()
   self.lcd_device.write(0x0)
  else:
   self.lcd_device.write((cmd >> 4))
   self.lcd_strobe()
   self.lcd_device.write((cmd & 0x0F))
   self.lcd_strobe()
   self.lcd_device.write(0x0)

 # write a character to lcd (or character rom)
 def lcd_write_char(self, charvalue):
  if self.reverse == 1:
   self.lcd_device.write((0x01 | (charvalue >> 4)<<4))
   self.lcd_strobe()
   self.lcd_device.write((0x01 | (charvalue & 0x0F)<<4))
   self.lcd_strobe()
   self.lcd_device.write(0x0)
  if self.reverse == 2:
   self.lcd_device.write((0x04 | (charvalue >> 4)<<4))
   self.lcd_strobe()
   self.lcd_device.write((0x04 | (charvalue & 0x0F)<<4))
   self.lcd_strobe()
   self.lcd_device.write(0x0)
  else:
   self.lcd_device.write((0x40 | (charvalue >> 4)))
   self.lcd_strobe()
   self.lcd_device.write((0x40 | (charvalue & 0x0F)))
   self.lcd_strobe()
   self.lcd_device.write(0x0)

 # put char function
 def lcd_putc(self, char):
  self.lcd_write_char(ord(char))

 # put string function
 def lcd_puts(self, string, line):
  if line == 1:
   self.lcd_write(0x80)
  if line == 2:
   self.lcd_write(0xC0)
  if line == 3:
   self.lcd_write(0x94)
  if line == 4:
   self.lcd_write(0xD4)

  for char in string:
   self.lcd_putc(char)

 # clear lcd and set to home
 def lcd_clear(self):
  self.lcd_write(0x1)
  self.lcd_write(0x2)

 # add custom characters (0 - 7)
 def lcd_load_custon_chars(self, fontdata):
  self.lcd_device.bus.write(0x40);
  for char in fontdata:
   for line in char:
    self.lcd_write_char(line)


class lcd:
		
    # LCD Address
    ADDRESS = 0x3f

    # commands
    LCD_CLEARDISPLAY = 0x01
    LCD_RETURNHOME = 0x02
    LCD_ENTRYMODESET = 0x04
    LCD_DISPLAYCONTROL = 0x08
    LCD_CURSORSHIFT = 0x10
    LCD_FUNCTIONSET = 0x20
    LCD_SETCGRAMADDR = 0x40
    LCD_SETDDRAMADDR = 0x80

    # flags for display entry mode
    LCD_ENTRYRIGHT = 0x00
    LCD_ENTRYLEFT = 0x02
    LCD_ENTRYSHIFTINCREMENT = 0x01
    LCD_ENTRYSHIFTDECREMENT = 0x00

    # flags for display on/off control
    LCD_DISPLAYON = 0x04
    LCD_DISPLAYOFF = 0x00
    LCD_CURSORON = 0x02
    LCD_CURSOROFF = 0x00
    LCD_BLINKON = 0x01
    LCD_BLINKOFF = 0x00

    # flags for display/cursor shift
    LCD_DISPLAYMOVE = 0x08
    LCD_CURSORMOVE = 0x00
    LCD_MOVERIGHT = 0x04
    LCD_MOVELEFT = 0x00

    # flags for function set
    LCD_8BITMODE = 0x10
    LCD_4BITMODE = 0x00
    LCD_2LINE = 0x08
    LCD_1LINE = 0x00
    LCD_5x10DOTS = 0x04
    LCD_5x8DOTS = 0x00

    # flags for backlight control
    LCD_BACKLIGHT = 0x08
    LCD_NOBACKLIGHT = 0x00

    En = 0b00000100 # Enable bit
    Rw = 0b00000010 # Read/Write bit
    Rs = 0b00000001 # Register select bit
		
   #initializes objects and lcd
   def __init__(self):
	  self.lcd_device = i2c_lib.i2c_device(ADDRESS)

	  self.lcd_write(0x03)
	  self.lcd_write(0x03)
	  self.lcd_write(0x03)
	  self.lcd_write(0x02)

	  self.lcd_write(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE)
	  self.lcd_write(LCD_DISPLAYCONTROL | LCD_DISPLAYON)
	  self.lcd_write(LCD_CLEARDISPLAY)   
	  self.lcd_write(LCD_ENTRYMODESET | LCD_ENTRYLEFT)
	  sleep(0.2)   

   # clocks EN to latch command
   def lcd_strobe(self, data):
	  self.lcd_device.write_cmd(data | En | LCD_BACKLIGHT)
	  sleep(.0005)
	  self.lcd_device.write_cmd(((data & ~En) | LCD_BACKLIGHT))
	  sleep(.0001)   

   def lcd_write_four_bits(self, data):
	  self.lcd_device.write_cmd(data | LCD_BACKLIGHT)
	  self.lcd_strobe(data)

   # write a command to lcd
   def lcd_write(self, cmd, mode=0):
	  self.lcd_write_four_bits(mode | (cmd & 0xF0))
	  self.lcd_write_four_bits(mode | ((cmd << 4) & 0xF0))

   # put string function
   def lcd_display_string(self, string, line):
	  if line == 1:
		 self.lcd_write(0x80)
	  if line == 2:
		 self.lcd_write(0xC0)
				if line == 3:
						self.lcd_write(0x94)
				if line == 4:
						self.lcd_write(0xD4)

	  for char in string:
		 self.lcd_write(ord(char), Rs)

   # clear lcd and set to home
   def lcd_clear(self):
	  self.lcd_write(LCD_CLEARDISPLAY)
	  self.lcd_write(LCD_RETURNHOME)
          
          
if __name__ == "__main__":
    print "I2C module"

    s = i2c_system()
    if s.status()=="Not Installed":
        r = raw_input("Do you wish to install ? (Y/N)")
        if r in "Yy":
            s.install()
            print "You will need to reboot the machine"

    # import pylcdlib
    # lcd = lcd(0x21,0)
    # lcd.lcd_puts("Raspberry Pi",1)  #display "Raspberry Pi" on line 1
    # lcd.lcd_puts("  Take a byte!",2)  #display "Take a byte!" on line 2

  
