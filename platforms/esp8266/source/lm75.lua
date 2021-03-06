---
-- @description Provides an interface to the LM75 I2C Temperature Sensor
-- LM75B Datasheet: http://www.ti.com/lit/ds/symlink/lm75b.pdf
-- Tested on NodeMCU 0.9.5 build 20150213.
-- @date April 08, 2015
-- @author David Lyon 
--  GitHub: https://github.com/clixx-io 
--  Website: http://clixx.io
--------------------------------------------------------------------------------
local moduleName = ...
local M = {}
_G[moduleName] = M

-- Constant device address.
local address = 72                            -- A2, A1, A0 = 0 -> GND
local temp_reg = 0
-- Default value for i2c communication
local bus = 0

---
--! @name init
--! @description Sets the LM75 device address's last three bits. 
--  Note: The address is defined as binary 0100[A2][A1][A0] where 
--  A2, A1, and A0 are defined by the connection of the pins, 
--  e.g. if the pins are connected all to GND then the paramter address 
--  will need to be 0x0.
-- @param address The 3 least significant bits (LSB) of the address
-- @param pinSDA The pin to use for SDA
-- @param pinSCL The pin to use for SCL
-- @param speed The speed of the I2C signal
-- @return void
---------------------------------------------------------------------------
function M.init(self, sda, scl)
   i2c.setup(bus, sda, scl, i2c.SLOW)
end

---
-- @name readTemp
-- @description reads the Temperature the Sensor
-- @return A String of data representing the temperature
----------------------------------------------------------
function M.readTemp(self)
 i2c.start(bus)
 i2c.address(bus, address, i2c.TRANSMITTER)
 i2c.write(bus, temp_reg)
 i2c.stop(bus)

 i2c.start(bus)
 i2c.address(bus, address, i2c.RECEIVER)
 c=i2c.read(bus, 2)
 i2c.stop(bus)

 h,l = string.byte(c,1,2)
 if h > 127 then h = h - 255 end        -- negative values - 2 complement representation
 if l > 127 then l = 5 else l = 0 end   -- LSB in only 0.5C
 return string.format("%d.%d", h,l)
end

return M
