
local moduleName = ...
local M = {}
_G[moduleName] = M 

local mcp = require ("mcp23008")

local lines = 16
local rows = 2

local backlight = 0

-- MCP23008 Register Numbers
local IODIR = 0x00
local IPOL  = 0x01
local GPPU  = 0x06
local GPIO  = 0x09

local function sendLcdToI2C(rs, e, data)
	local value = bit.lshift(rs, 1)
    value = value + bit.lshift(e, 2)
    value = value + bit.rshift(data, 1)
    value = value + backlight
	mcp.writeGPIO(value)
end

local function sendLcdRaw(rs, data)
	sendLcdToI2C(rs, 1, data)
	sendLcdToI2C(rs, 0, data)
end

local function sendLcd(rs, data)
	sendLcdRaw(rs, bit.band(data, 0xf0)) -- high nibble
	sendLcdRaw(rs, bit.lshift(bit.band(data, 0x0f), 4))  -- low nibble
end

function M.begin(pinSDA,pinSCL,lcdCols,lcdLines,address)
	address = address or 0
    lines = lcdLines
    cols = lcdCols

--    # Set up the IO expander
--    wiringpi2.wiringPiI2CWriteReg8(self.device, self.GPIO,  0x00) # Clear outputs
--    wiringpi2.wiringPiI2CWriteReg8(self.device, self.IODIR, 0x00) # Direction
--    wiringpi2.wiringPiI2CWriteReg8(self.device, self.GPPU,  0x00) # Pull ups
--    wiringpi2.wiringPiI2CWriteReg8(self.device, self.IPOL,  0x00) # Polarity
--    # Initialise the display in 4 bit mode
--   self._writeLCD(0x33, True)
--    self._writeLCD(0x32, True)
--    self._writeLCD(0x28, True)
--    # Set up initial state
--    self._writeLCD(0x0C, True)
--    self._writeLCD(0x06, True)
--    self._writeLCD(0x01, True)

    mcp.begin(address,pinSDA,pinSCL,i2c.SLOW)
    mcp.writeGPIO(0x00) -- make all GPIO pins as outputs
    mcp.writeIODIR(0x00) -- make all GPIO pins as outputs
    mcp.writeGPPU(0x00)  -- make all pins not Pullups

    -- Initialise the display in 4 bit mode
--   self._writeLCD(0x33, True)
--    self._writeLCD(0x32, True)
--    self._writeLCD(0x28, True)
    sendLcdRaw(1,0x33)
    sendLcdRaw(1,0x32)
    sendLcdRaw(1,0x28)
--    # Set up initial state
--    self._writeLCD(0x0C, True)
--    self._writeLCD(0x06, True)
--    self._writeLCD(0x01, True)
    sendLcdRaw(1,0x0C)
    sendLcdRaw(1,0x06)
    sendLcdRaw(1,0x01)

end

function M.write(ch)
    sendLcd(1, string.byte(ch, 1))
end

function M.print(s)
    for i = 1, #s do
        sendLcd(1, string.byte(s, i))
    end
end

M.ROW_OFFSETS = {0, 0x40, 0x14, 0x54}

function M.setCursor(col, row)
    local val = bit.bor(0x80, col, M.ROW_OFFSETS[row + 1])
    sendLcd(0, val)
end

function M.setBacklight(b)
    backlight = bit.lshift(b, 7)
    mcp.writeGPIO(backlight)
end

return M
