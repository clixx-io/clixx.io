# / Usr / bin / python
# - * - Encoding: utf-8 - * -

# Controlling TEA5767 FM receiver via I2C communication
# Last modified November, 2013

import smbus # import operations with I2C
import time # operations with time (pause)
import sys # DETECTION command line parameter
import clixxIO_I2C

pin_tlm = 13 # pin26 = GPIO7 (left button - the reading rate)
pin_tlp = 7 # pin24 = GPIO8 (right click - attributed to frequency)

class fmreceiver():

  # ================================================
  # Initialise and set up some stations
  def __init__(self):

    self.bus = smbus.SMBus (1) # Newer RASP (512 megabytes)
    # Bus = smbus.SMBus (0) # older versions RASP (256MB)

    # A list of stations and their frequencies
    self.Stations = { 0 : [96.1, "One-FM"],
                      1 : [96.9, "Nova"],
                      2 : [101.7, "WFS FM"],
                      3 : [102.5, "2MBS FM"],
                      4 : [103.2, "FM 103.2"],
                      5 : [104.1, "2-Day FM"],
                      6 : [104.9, "TRIPLE M"],
                      7 : [106.4, "Czech Radio"] }

    self.freq = 87.5
    self.station = 0
    self.strength = 0

  def next_preset(self):

    self.station += 1
    if self.station >= len(self.Stations):
      self.station = 0

    return self.tune_f(self.Stations[self.station][0],self.Stations[self.station][1])

  # ================================================
  # Subroutine for prime setting the desired frequency
  def tune_f (self,freq, stationname=''):

    if (freq >= 87.5 and freq <= 108): # when frequency is within acceptable limits, subscribe to the circuit

      # Conversion rate at two bytes (according to Application Instructions)
      freq14bit = int (4 * (freq * 1000000 + 225000) / 32768)
      freqH = int (freq14bit / 256)
      freqL = freq14bit & 0xFF

      # Description of each bit in a byte - viz. Product Sheet
      bajt0 = 0x60 # I2C address circuit
      bajt1 = freqH # 1.bajt (MUTE bit; frequency H)
      bajt2 = freqL # 2.bajt (frequency L)
      bajt3 = 0b10110000 # 3.bajt (SUD; SSL1, SSL2; HLSI, MS, MR, ML; SWP1)
      bajt4 = 0b00010000 # 4.bajt (SWP2; STBY, BL; XTAL; sad; HCC; SNC, SI)
      bajt5 = 0b00000000 # 5.bajt (PLREFF; DTC, 0, 0, 0, 0, 0, 0)

      blokdat = [bajt2, bajt3, bajt4, bajt5]
      self.bus.write_i2c_block_data (bajt0, bajt1, blokdat) # Set new frequency to the circuit

      time.sleep (0.1) # wait a while before signal strength is evaluated

      bajt1r = self.bus.read_byte (bajt0) # read the first byte (to be read independently)
      data = self.bus.read_i2c_block_data (bajt0, bajt1) # read data from the circuit for the detected signal level
      data[0] = bajt1r # unread messages replace the first byte separately retrieve the value

      print "frequency =" + str (freq) + "MHz", "\t DATA:" + str (data [0:5]) + "\t (signal strength:" + str (data [3] >> 4) + ")"

      self.freq = freq
      self.strength = data [3] >> 4

    return ([stationname,self.freq,self.strength])

  def frequency():
    return self.freq

  # ================================================
  # Subroutine to Search for the nearest station from a specified frequency
  def scan(self, freq, up_direction=True, adc_limit=10):

    if (adc_limit> 15 or adc_limit <0): # evaluated whether the specified limit at between 0 and 15
      adc_limit = 7 # if it is outside, is automatically set to 7

    if (adc_limit != 7): # When the auto scan of the whole band prints info
      print "Displaying the frequencies with minimum signal strength" + str (adc_limit)

    # Main loop to check if the frequency is within acceptable limits
    self.freq = freq
    while (self.freq >= 87.5 and self.freq <= 108): # when the frequency is out of range, the loop terminates

      if (up_direction == True): # according to the scan direction to either add or remove 100kHz
        self.freq = self.freq + 0.1
      else:
        self.freq = self.freq - 0.1

      # Conversion rate at two bytes (according to Application Instructions)
      freq14bit = int (4 * (freq * 1000000 + 225000) / 32768)
      freqH = int (freq14bit / 256)
      freqL = freq14bit & 0xFF

      mutebit = 0b00000000 # pri vyhledavani turn on the sound (hum / sumi / sand like a normal radio when debugging)
    # Mutebit = 0b10000000 # pri vyhledavani off the volume

      # Description of each bit in a byte - viz. Product Sheet
      bajt0 = 0x60 # I2C address circuit
      bajt1 = freqH | mutebit # 1.bajt (MUTE bit; frequency H)
      bajt2 = freqL # 2.bajt (frequency L)
      bajt3 = 0b10110000 # 3.bajt (SUD; SSL1, SSL2; HLSI, MS, MR, ML; SWP1)
      bajt4 = 0b00010000 # 4.bajt (SWP2; STBY, BL; XTAL; sad; HCC; SNC, SI)
      bajt5 = 0b00000000 # 5.bajt (PLREFF; DTC, 0, 0, 0, 0, 0, 0)

      blokdat = [bajt2, bajt3, bajt4, bajt5]

      # Tuned to a new frequency
      self.bus.write_i2c_block_data (bajt0, bajt1, blokdat)

      time.sleep (0.05) # respective frequencies wait awhile until evaluates signal strength

      # Read the contents of all registers
      bajt1r = self.bus.read_byte (bajt0) # The first byte must be read independently
      data = self.bus.read_i2c_block_data (bajt0, bajt1) # load all the bytes from the circuit
      data[0] = bajt1r # first byte of the refund separately retrieve the value

      strength = data [3] >> 4 # in the highest 4 bits of the fourth baju (data [3]) when reading registry signal level
 
      if (strength >= adc_limit): # Minimum signal strength, which will be considered for a tuned station (0 to 15)
        print "f =" + str (self.freq) + "MHz", "\tDATA:" + str (data [0:5]) + "\t (signal strength:" + str (strength) + ")"

        if (mutebit == 0b10000000): # Disable Bit of a possible MUTE when station
          bajt1 = bajt1 & 0b01111111
          self.bus.write_i2c_block_data (bajt0, bajt1, blokdat)
 
        # if (find_1_freq == True): # When looking only one of the closest frequency, so after finding exits the while loop

        break

      if (self.freq> 108): # If you flick through the upper end zone ...
        self.freq = 87.5 # ... So at the end of the back ...
        print "Reached the high end of the band" # and reports an, at the end of
      if (self.freq <87.5): # when you move below the lower end zone ...
        self.freq = 87.5 # ... So at the end of the back ...
        print "reached the bottom end of the band" # and reports an, at the end of

    return (self.freq)

  # ================================================
  # Subroutine to mute the output
  def mute(self):
    bajt1 = self.bus.read_byte (0x60)
    bajt1 = bajt1 | 0b10000000
    self.bus.write_byte (0x60, bajt1)

  # ================================================
  # Subroutine to unmute the output
  def unmute(self):
    bajt1 = self.bus.read_byte (0x60)
    bajt1 = bajt1 & 0b01111111
    self.bus.write_byte (0x60, bajt1)

  # ================================================
  # Subroutine to search the station using the buttons
  def button (self):

    import RPi.GPIO as GPIO # GPIO output in RASP

    GPIO.setwarnings (False)
    GPIO.setmode (GPIO.BOARD) # numbering according hardwerovych pin (1 to 26)

    # Buttons are normally closed contact, a common ports have to GND,
    # The remaining pins are connected to GPIO7 (pin26) and GPIO8 (pin24)

    # Setting the appropriate GPIO pin as input with pull-up resistors
    GPIO.setup (pin_tlm, GPIO.IN, pull_up_down = GPIO.PUD_UP)
    GPIO.setup (pin_tlp, GPIO.IN, pull_up_down = GPIO.PUD_UP)

    tl_minus = 0
    tl_plus = 0
 
    print "... search - >> - >> - >>"
    freq = scan (107.5, True) # When you run a subroutine to find the first station below

    # Main loop to test two of buttons OPENING
    while ((tl_minus == 0) or (tl_plus == 0)): # When both are pressed, or when the error, the loop terminates

      tl_minus = GPIO.input (pin_tlm) # Read the buttons on GPIO pins
      tl_plus = GPIO.input (pin_tlp)

      if (tl_plus == 1): # when pressing the PLUS button, the search for the nearest higher station
        print "... search - >> - >> - >>"
        time.sleep (0.5)
        if (tl_minus == 0): # if it is not pressed at the same time and Minus buttons ...
          freq = scan (freq, True) # Find the nearest higher frequency
 
      if (tl_minus == 1): # by pressing MINUS buttons to seek the next lower station
        print "Search ... << - << - << -"
        time.sleep (0.5)
        if (tl_plus == 0): # if it is not pressed at the same time and PLUS ... button
          freq = scan (freq, False) # Find the nearest lower frequency

      time.sleep (0.1)

  # ================================================
  # Subroutine to switch preset stations using the buttons
  def Switches (self):

    import RPi.GPIO as GPIO # GPIO output in RASP

    GPIO.setwarnings (False)
    GPIO.setmode (GPIO.BOARD) # numbering according hardwerovych pin (1 to 26)

    # Buttons are normally closed contact, a common ports have to GND,
    # The remaining pins are connected to GPIO7 (pin26) and GPIO8 (pin24)

    # Setting the appropriate GPIO pin as input with pull-up resistors
    GPIO.setup (pin_tlm, GPIO.IN, pull_up_down = GPIO.PUD_UP)
    GPIO.setup (pin_tlp, GPIO.IN, pull_up_down = GPIO.PUD_UP)

    tl_minus = 0
    tl_plus = 0

    # The variable "float (station [index] [0])" is a radio station with the relevant index
    # The variable "station [index] [1]" is the name of the station with the relevant index

    pocet_stanic = len (self.Stations)

    index = 0 # When you run the subroutine sets the frequency at zero station
    print self.Stations[index][1]
    tune_f (self.Stations[index][0])

    # Main loop to test two of buttons OPENING
    while ((tl_minus == 0) or (tl_plus == 0)): # When both are pressed, or when the error, the loop terminates
      tl_minus = GPIO.input (pin_tlm) # Read the buttons on GPIO pins
      tl_plus = GPIO.input (pin_tlp)

      if (tl_plus == 1): # PLUS when pressing the button switches to the next channel in the list
        time.sleep (0.5)
        if (tl_minus == 0): # if it is not pressed at the same time and Minus buttons ...
          index = index + 1 # ... Move to the next station index
          if (index> (pocet_stanic-1)): # Then, when the index is greater than the number of stations ...
            index = 0 # the default is index on top of the list
 
          print self.Stations[index][1]
          tune_f (self.Stations[index] [0]) # Set the frequency of the current station


      if (tl_minus == 1): # by pressing the minus button is switched to the next channel in the list
        time.sleep (0.5)
        if (tl_plus == 0): # if it is not pressed at the same time and PLUS ... button
          index = index - 1 # ... Moves to the previous station index
          if (index <0): # when index "Podleze" the first station ...
            index = (pocet_stanic-1) # ... adjustable with the index on the last station on the list

          print self.Stations[index][1]
          tune_f (self.Stations[index][0]) # Set the frequency of the current station

      time.sleep (0.1)

# ================================================
# Beginning of the program - evaluation of command line parameters
# ================================================
if __name__ == "__main__":

  try: # If the first parameter is missing, the HELP
    parameter = sys.argv [1]
  except:
    parameter = ""

  fmradio = fmreceiver()

  if (parameter == "-f"): # prime set to a specific frequency
    try:
      freq = float (sys.argv [2])
    except:
      freq = 0
    tune_f(freq)

  elif (parameter == "-up"): # automatic scanning "UP" from a specified frequency
    try:
      freq = float (sys.argv [2])
    except:
      freq = 87.5 # If a parameter is missing or is misspelled, set the lower limit of the
    fmradio.scan(freq, True) # scan from a specified frequency UP

  elif (parameter == "-down"): # automatic scanning "down" from a specified frequency
    try:
      freq = float (sys.argv [2])
    except:
      freq = 108 # If a parameter is missing or is misspelled, set the upper limit of the range
    fmradio.scan(freq, False) # scan from a specified frequency DOWN

  elif (parameter == "-l"): # will automatically list all strong frequency
    fmradio.scan (87.5, True) # scan from the beginning Strips UP

  elif (parameter == "-mute"): # Mute the volume
    fmradio.mute()

  elif (parameter == "-restore"): # restore volume
    fmradio.unmute()

  elif (parameter == "-manual"): # use the buttons on the GPIO to automatically find the nearest station
    fmradio.button()

  elif (parameter == "-presets"): # use the buttons on the GPIO to switch preset stations
    fmradio.Switches ()

  else: # If the first parameter has not found any variant, help is displayed
    print "clixx.io FM-Radio Module"
    print 
    print "-n ... fff.f prime frequency setting"
    print "-sn ... fff.f LL find the nearest station is above a certain frequency"
    print "...-sd fff.f LL find the nearest station to the specified frequency"
    print "-v ... LL will automatically list all strong rate"
    print "...-h0 completely mute the volume (MUTE)"
    print "...-h1 restore volume"
    print "... t-LL channel search using the buttons on GPIO ports"
    print "-p ... Alternating predefined stations using"
    print ""
    print "fff.f = frequency in MHz. Allowable values ​​are between 87.5 and 108"
    print "LL = minimum signal strength (ADC_level) at which the signal"
    print "auto search CONSIDERED station (0 to 15)"

