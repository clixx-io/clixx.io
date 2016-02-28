----------------------------------------------------------------
--
-- Clixx.IO 
--
-- Drive twelve (or less) WS2812 RGB-LEDs into a test pattern
--
-- (c) 2016 David Lyon
--
----------------------------------------------------------------

-- ESP-01 GPIO Mapping
gpio_0i, gpio_0o = 4,3

-- We write to 12x LEDs in Series with a Colour Pattern
ws2812.writergb(gpio_0o,string.char(0,0,255,255,0,0,255,0,0,255,0,0,255,255,0,255,255,0,255,255,0,255,255,0,0,255,0,0,255,0,0,255,0,0,255,255))
