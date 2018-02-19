# /usr/bin/env python
import smbus    # import operations with I2C
from time import sleep
import clixxIO_I2C

#----------------------------------------------------------------------------
# Description
#----------------------------------------------------------------------------

class TemperatureSensor():

  def __init__(self, addr=0x48, port=1):
    """ Constructor
    """
    self.bus = clixxIO_I2C.i2c_device(addr, port)

  def tempC(self):
    """ Return temperature in Celsius
    """
    data = self.bus.read_nbytes_data(0,2)

    TempMSB = data[0]
    TempLSB = data[1]

    temp_y = (((TempMSB << 8) | TempLSB) >>7) * 0.5

    if temp_y > 125:

        temp_y = (((((TempMSB << 8) | TempLSB) >>7) * 0.5) -256)

    return temp_y

if __name__ == "__main__":

    try:
        print "Connecting to Temperature sensor via I2C bus"

        t = TemperatureSensor()

        print "Temperature is %g degrees celsius" % t.tempC()

    except:

        print "Error occurred connecting to the device. Is it installed ? and are permissions set correctly?"

