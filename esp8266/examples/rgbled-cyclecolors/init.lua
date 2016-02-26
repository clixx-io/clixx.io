----------------------------------------------------------------
--
-- Clixx.IO Example program to cycle through some RGB Colours
--
-- (c) 2016 David Lyon
----------------------------------------------------------------
print("clixx.io RGB LED Colour Cycling Example (c) 2016 clixx.io Pty Limited" )

coloridx=1
numcolors=0
colorvals = {
             -- R & G & B
             {0,255,255},
             {255,255,0},
             {255,0,255},
             {0,0,0},

             -- Blue's
             {0,131,215},
             {0,153,255},
             {0,102,204},     -- darker with more blue again
             {0,51,153},      -- darker with more blue
             {62,154,222},
             {51,51,102},     -- Brighter Cyan

             -- Green Group
             {0,5,10},
             {0,10,10},
             {0,25,10},
             {0,255,40},
             {0,255,30},
             {0,255,20},
             {10,255,20},
             {20,255,20},
             {30,255,20},
             {0,86,10},
             {0,102,0},
             {0,153,0},
             {0,202,0},
             {0,255,0},

             -- Pinks
             {255,153,51},
             {255,121,75},
             {255,219,157},
             {255,204,102},
             {255,255,153},

             -- Orange Group
             {153,102,0},     -- Lime Yellow
             {204,153,0},     -- Yellow Orange
             {255,204,0}, 
             {255,51,0},
             {255,0,0},
            
             -- Whites
             {153,204,255},
             {180,226,255},
             {222,255,255},
             {255,204,255},
             {204,204,255},
             {153,153,255},
             {102,102,204},
             {153,153,204},
             {102,102,153},
             {102,204,51},
             {153,255,102},
             {204,255,204}

            }

for _ in pairs(colorvals) do numcolors = numcolors + 1 end

-- This timer loop cycles through the list of colours
tmr.alarm(0,1000,1,function()
  coloridx = coloridx +1
  if coloridx > numcolors then
    coloridx = 1
    end
  ws2812.writergb(3, string.char(colorvals[coloridx][1],colorvals[coloridx][2],colorvals[coloridx][3]))
end)
