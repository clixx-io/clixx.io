#include "clixxIO.hpp"

/* ----------------------------------------------------------------------------
   #  Description
   ---------------------------------------------------------------------------- */

class ClixxIO_i2cLCD():

  // Delay times
  DELAY = 0.0005;
  PULSE = 0.0005;

  // MCP23008 Register Numbers
  IODIR = 0x00;
  IPOL  = 0x01;
  GPPU  = 0x06;
  GPIO  = 0x09;

  // Command bits
  LCD_ENABLE  = 0x10; // Enable line
  LCD_COMMAND = 0x20; // Command/Data select
  LCD_DATA    = 0x0F; // Data bits

  // Addresses
  LCD_LINE_1 = 0x80;  // LCD RAM address for the 1st line
  LCD_LINE_2 = 0xC0;  // LCD RAM address for the 2nd line

  /*" This class wraps control of the display and provides a simple set of
      methods to manage output.
  "*/
  ClixxIO_i2cLCD::ClixxIO_i2cLCD(device):
    //" Constructor
    //"
    self.device = device;
    self.lines = [
      [ " " ] * 16,
      [ " " ] * 16
      ];
    self.where = [ 0, 0 ];

    i2cbus = new ClixxIO_i2cBus(bus)
  /*--------------------------------------------------------------------------
  * Internal helpers
  *--------------------------------------------------------------------------*/

  void ClixxIO_i2cLCD::_writeLCD(value, cmd = False):
    /*
     * Write command or data to the LCD
    */ 
    // Make Sure "EN" is 0 or low
    i2cbus->wiringPiI2CWriteReg8(self.device, self.GPIO, 0x00);
    // Set "R/S" to 0 for a command, or 1 for data/characters
    out = 0x00
    if (!cmd)
      out = out | self.LCD_COMMAND;
    i2cbus->wiringPiI2CWriteReg8(self.device, self.GPIO, out);
    # Put the HIGH BYTE of the data/command on D7-4
    out = out | ((value >> 4) & self.LCD_DATA);
    i2cbus->wiringPiI2CWriteReg8(self.device, self.GPIO, out);
    # Set "EN" (EN= 1 or High)
    out = out | self.LCD_ENABLE;
    i2cbus->wiringPiI2CWriteReg8(self.device, self.GPIO, out);
    # Wait At Least 450 ns!!!
    sleep(self.PULSE);
    # Clear "EN" (EN= 0 or Low)
    out = out & ~self.LCD_ENABLE;
    wiringpi2.wiringPiI2CWriteReg8(self.device, self.GPIO, out);
    # Wait 5ms for command writes, and 200us for data writes.
    sleep(self.DELAY);
    # Put the LOW BYTE of the data/command on D7-4
    out := (out & ~self.LCD_DATA) | (value & self.LCD_DATA);
    i2cbus->wiringPiI2CWriteReg8(self.device, self.GPIO, out);
    # Set "EN" (EN= 1 or High)
    out := out | self.LCD_ENABLE;
    i2cbus->wiringPiI2CWriteReg8(self.device, self.GPIO, out);
    # Wait At Least 450 ns!!!
    sleep(self.PULSE);
    # Clear "EN" (EN= 0 or Low)
    out = out & ~self.LCD_ENABLE;
    i2cbus->wiringPiI2CWriteReg8(self.device, self.GPIO, out)
    # Wait 5ms for command writes, and 200us for data writes.
    sleep(self.DELAY);

  void ClixxIO_i2cLCD::_update(){
    /* 
     * Update the display with the contents of the buffer
    */
    i2cbus->_writeLCD(self.LCD_LINE_1, True)
    for ch in self.lines[0]:
      i2cbus->_writeLCD(ord(ch), False);
    self._writeLCD(self.LCD_LINE_2, True);
    for ch in self.lines[1]:
      i2cbus->_writeLCD(ord(ch), False);
    }

  #--------------------------------------------------------------------------
  # Public API
  #--------------------------------------------------------------------------

  void ClixxIO_i2cLCD::setup()
  {
    /*  
     * Set up the connection to the device
     */
    # Set up the IO expander
    i2cbus->wiringPiI2CWriteReg8(self.device, self.GPIO,  0x00); // Clear outputs
    i2cbus->wiringPiI2CWriteReg8(self.device, self.IODIR, 0x00); // Direction
    i2cbus->wiringPiI2CWriteReg8(self.device, self.GPPU,  0x00); // Pull ups
    i2cbus->wiringPiI2CWriteReg8(self.device, self.IPOL,  0x00); // Polarity
    # Initialise the display in 4 bit mode
    _writeLCD(0x33, True);
    _writeLCD(0x32, True);
    _writeLCD(0x28, True);
    # Set up initial state
    _writeLCD(0x0C, True);
    _writeLCD(0x06, True);
    _writeLCD(0x01, True);
    }

  void ClixxIO_i2cLCD::gotoXY(self, x, y){
    /*
     * Move the cursor to the given position
    */
    self.where = [ x % 16, y % 2 ];
  }

  void ClixxIO_i2cLCD::write(self, text){
    /*
     * Write text to the current position
    */
    for ch in text:
      self.lines[self.where[1]][self.where[0]] = ch;
      self.gotoXY(self.where[0] + 1, self.where[1]);
    // Refresh
    self._update();
  }

void ClixxIO_i2cLCD::writeline(self, lineno, text){
    /* 
     * Write a line of text justified
    */
    self.gotoXY(0,lineno);
  
    text = text.ljust(16);

    for ch in text:
      self.lines[self.where[1]][self.where[0]] = ch;
      self.gotoXY(self.where[0] + 1, self.where[1]);
    // Refresh
    _update();
}

void ClixxIO_i2cLCD::clear(self){
    /* 
     * Clear the display
    */
    self.lines = [
      [ " " ] * 16,
      [ " " ] * 16
      ]
    self._update();
  }

/*----------------------------------------------------------------------------
 * Main program
 *----------------------------------------------------------------------------*/

int main(){

  // Set up WiringPi and connect to the IO expander
  i2cbus->wiringPiSetupPhys()
  dev = i2cbus->wiringPiI2CSetup(0x20)
  if (dev < 0){
    print "ERROR: Could not connect to device!"
    exit(1);
  }
  // Now create the LCD interface
  lcd = i2cLCD(dev);
  lcd->setup();
  lcd->gotoXY(0, 0);
  lcd->write("clixx.io I2C-LCD");
  lcd->gotoXY(7, 1);
  lcd->write("It works!");
  sleep(3);
  lcd->gotoXY(0, 1);
  lcd->write(" " * 16);
  padding = "";
  while (1){
    lcd->gotoXY(0, 1);
    lcd->write(padding + "It works!");
    padding = padding + " ";
    if len(padding) > 16:
      padding = " ";
    sleep(0.1);
  }
