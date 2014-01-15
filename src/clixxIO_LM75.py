# /usr/bin/env python
import smbus    # import operations with I2C
from time import sleep
import clixxIO_I2C

#----------------------------------------------------------------------------
# Description
#----------------------------------------------------------------------------

class TempLM75():

  def __init__(self, addr=1, port=0x48):
    """ Constructor
    """
    self.bus = device(addr, port)

  def tempC():
	""" Return temperature in Celsius
	"""
	data = self.bus.read_i2c_block_data(0x48, 0)

	TempMSB = data[0]
	TempLSB = data[1]

	temp_y = (((TempMSB << 8) | TempLSB) >>7) * 0.5

	if temp_y > 125:

		temp_y =(((((TempMSB << 8) | TempLSB) >>7) * 0.5) -256)

if __name__ == "__main__":

	t = i2cTempLM75()
	
