I2C Temperature Sensor
----------------------

.. image:: images/Temp-Board.png

This I2C Temperature Sensor is based around the LM75 Integrated circuit
and provides a sensor that runs off the I2C bus meaning that you can
share the pins of with multiple devices.

Connection
----------

To be used, this device needs to be plugged into the I2C bus interface on your
microprocessor. It can also be daisy-chained along with other I2C devices.


Code Examples
-------------

.. code-block:: python

    '''
      Temp
      Displays the temperature.

      This example code is in the public domain.
    '''

    import clixxIO_LM75

    if __name__ == "__main__":
    
        t = clixxIO_LM75.TempSensor()

        print "Temperature is %f degrees celcius" % t.tempC()
        
Linux
-----

This device can be detected via the command line using the i2cdetect command.

run 'i2cdetect -y 1' and it should appear as address 0x48

Changing the I2C Address
------------------------

The board by default comes with a preset I2C address of 0x48. For most users
this will suffice.

In situations where you want to run more temperature sensors from the one bus
it's possible to modify the hardware adress by doing some simple modifications
to the circuit board.

Here are the steps:

 - Use an engraving tool to cut across the between the bottom and middle
   lines of the address sgit elector
   
 - Join the pads with solder to select the address that you want
 
 - The middle pads go to the address selector on the IC
 
 - The top pads go to + voltage
 
 - The bottom pads go to - voltage.
 





 
