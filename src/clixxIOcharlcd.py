# /usr/bin/env python
import smbus    # import operations with I2C
from time import sleep
import clixxIO_I2C
import wiringpi2

#----------------------------------------------------------------------------
# Description
#----------------------------------------------------------------------------

class i2cLCD():


  # Delay times
  DELAY = 0.0005
  PULSE = 0.0005

  # MCP23008 Register Numbers
  IODIR = 0x00
  IPOL  = 0x01
  GPPU  = 0x06
  GPIO  = 0x09

  # Command bits
  LCD_ENABLE  = 0x10 # Enable line
  LCD_COMMAND = 0x20 # Command/Data select
  LCD_DATA    = 0x0F # Data bits

  # Addresses
  LCD_LINE_1 = 0x80 # LCD RAM address for the 1st line
  LCD_LINE_2 = 0xC0 # LCD RAM address for the 2nd line

  """ This class wraps control of the display and provides a simple set of
      methods to manage output.
  """
  def __init__(self, device):
    """ Constructor
    """
    self.device = device
    self.lines = [
      [ " " ] * 16,
      [ " " ] * 16
      ]
    self.where = [ 0, 0 ]

  #--------------------------------------------------------------------------
  # Internal helpers
  #--------------------------------------------------------------------------

  def _writeLCD(self, value, cmd = False):
    """ Write command or data to the LCD
    """
    # Make Sure "EN" is 0 or low
    wiringpi2.wiringPiI2CWriteReg8(dev, self.GPIO, 0x00)
    # Set "R/S" to 0 for a command, or 1 for data/characters
    out = 0x00
    if not cmd:
      out = out | self.LCD_COMMAND
    wiringpi2.wiringPiI2CWriteReg8(dev, self.GPIO, out)
    # Put the HIGH BYTE of the data/command on D7-4
    out = out | ((value >> 4) & self.LCD_DATA)
    wiringpi2.wiringPiI2CWriteReg8(dev, self.GPIO, out)
    # Set "EN" (EN= 1 or High)
    out = out | self.LCD_ENABLE
    wiringpi2.wiringPiI2CWriteReg8(dev, self.GPIO, out)
    # Wait At Least 450 ns!!!
    sleep(self.PULSE)
    # Clear "EN" (EN= 0 or Low)
    out = out & ~self.LCD_ENABLE
    wiringpi2.wiringPiI2CWriteReg8(dev, self.GPIO, out)
    # Wait 5ms for command writes, and 200us for data writes.
    sleep(self.DELAY)
    # Put the LOW BYTE of the data/command on D7-4
    out = (out & ~self.LCD_DATA) | (value & self.LCD_DATA)
    wiringpi2.wiringPiI2CWriteReg8(dev, self.GPIO, out)
    # Set "EN" (EN= 1 or High)
    out = out | self.LCD_ENABLE
    wiringpi2.wiringPiI2CWriteReg8(dev, self.GPIO, out)
    # Wait At Least 450 ns!!!
    sleep(self.PULSE)
    # Clear "EN" (EN= 0 or Low)
    out = out & ~self.LCD_ENABLE
    wiringpi2.wiringPiI2CWriteReg8(dev, self.GPIO, out)
    # Wait 5ms for command writes, and 200us for data writes.
    sleep(self.DELAY)

  def _update(self):
    """ Update the display with the contents of the buffer
    """
    self._writeLCD(self.LCD_LINE_1, True)
    for ch in self.lines[0]:
      self._writeLCD(ord(ch), False)
    self._writeLCD(self.LCD_LINE_2, True)
    for ch in self.lines[1]:
      self._writeLCD(ord(ch), False)

  #--------------------------------------------------------------------------
  # Public API
  #--------------------------------------------------------------------------

  def setup(self):
    """ Set up the connection to the device
    """
    # Set up the IO expander
    wiringpi2.wiringPiI2CWriteReg8(dev, self.GPIO,  0x00) # Clear outputs
    wiringpi2.wiringPiI2CWriteReg8(dev, self.IODIR, 0x00) # Direction
    wiringpi2.wiringPiI2CWriteReg8(dev, self.GPPU,  0x00) # Pull ups
    wiringpi2.wiringPiI2CWriteReg8(dev, self.IPOL,  0x00) # Polarity
    # Initialise the display in 4 bit mode
    self._writeLCD(0x33, True)
    self._writeLCD(0x32, True)
    self._writeLCD(0x28, True)
    # Set up initial state
    self._writeLCD(0x0C, True)
    self._writeLCD(0x06, True)
    self._writeLCD(0x01, True)

  def gotoXY(self, x, y):
    """ Move the cursor to the given position
    """
    self.where = [ x % 16, y % 2 ]

  def write(self, text):
    """ Write text to the current position
    """
    for ch in text:
      self.lines[self.where[1]][self.where[0]] = ch
      self.gotoXY(self.where[0] + 1, self.where[1])
    # Refresh
    self._update()

  def clear(self):
    """ Clear the display
    """
    self.lines = [
      [ " " ] * 16,
      [ " " ] * 16
      ]
    self._update()

#----------------------------------------------------------------------------
# Main program
#----------------------------------------------------------------------------

if __name__ == "__main__":
  # Set up WiringPi and connect to the IO expander
  wiringpi2.wiringPiSetupPhys()
  dev = wiringpi2.wiringPiI2CSetup(0x20)
  if dev < 0:
    print "ERROR: Could not connect to device!"
    exit(1)
  # Now create the LCD interface
  lcd = i2cLCD(dev)
  lcd.setup()
  lcd.gotoXY(0, 0)
  lcd.write("clixx.io I2C-LCD")
  lcd.gotoXY(7, 1)
  lcd.write("It works!")
  sleep(3)
  lcd.gotoXY(0, 1)
  lcd.write(" " * 16)
  padding = ""
  while True:
    lcd.gotoXY(0, 1)
    lcd.write(padding + "It works!")
    padding = padding + " "
    if len(padding) > 16:
      padding = " "
    sleep(0.1)
