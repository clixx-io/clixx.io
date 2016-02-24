-- Simple Example to cycle through some RGB Colours
coloridx=1
numcolors=0
colorvals = {{255,0,0},
             {0,255,0},
             {0,0,255},
             {0,0,0},

             {0,255,255},
             {255,255,0},
             {255,0,255},
             {0,0,0},
             
             {153,102,0},
             {204,153,0},
             {255,204,0},
             {255,255,0},
             {255,255,153},
             {255,219,157},
             {255,204,102},
             {255,153,51},
             {255,121,75},
             {255,51,0},
             {153,0,0},

             {51,51,102},
             {0,51,153},
             {0,102,204},
             {0,131,215},
             {0,153,255},
             {62,154,222},
             {153,204,255},
             {180,226,255},
             {222,255,255},
             {255,204,255},
             {204,204,255},

             {153,153,255},
             {102,102,204},
             {153,153,204},
             {102,102,153},
             {0,102,0},
             {0,153,0},
             {102,204,51},
             {153,255,102},
             {204,255,204},

             {0,0,0}
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
