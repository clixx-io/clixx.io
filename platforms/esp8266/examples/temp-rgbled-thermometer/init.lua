----------------------------------------------------------------
--
-- Clixx.IO 
--
-- Change Colour on a WS2812 RGBLED based on temperature
--
-- (c) 2016 David Lyon
--
----------------------------------------------------------------

require('ds18b20')

port = 80

gpio_0i, gpio_0o = 4,3        -- ESP-01 GPIO Mapping

coloridx=1
colorvals = {
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
    }

numcolors = #colorvals

tempmappings = {
             -- A temperature threshold then an index into the rgb-table
             {-3,1},
             {0,3},
             {5,6},           
             {10,7},         
             {15,23},
             {20,25},         
             {25,22},     
             {30,21},     
             {35,28},     
             {40,29},     
     }            

ds18b20.setup(gpio_0i)

print("clixx.io RGB Temperature Controlled LED")

function statusReportHtml()
     -- Generate a Table showing the current temperature thresholds
     local mappingmarkup
     
     mappingmarkup = "<br/><table border=\"1\" style=\"width:35%\"><caption>Temperature Run Settings</caption>"
     mappingmarkup = mappingmarkup .. "<tr><th>Temperature</th><th>Color Index</th></tr>"
     for k,v in pairs(tempmappings) do
          if (v[2] == coloridx) then
               mappingmarkup = mappingmarkup .. "<tr bgcolor=\"#1E88E5\">" .. "<td>" .. v[1] .. "</td><td>" .. v[2] .. "</td></tr>"
          else
               mappingmarkup = mappingmarkup .. "<tr>" .. "<td>" .. v[1] .. "</td><td>" .. v[2] .. "</td></tr>"
               end     
          end
     mappingmarkup = mappingmarkup .. "</table>"
     return mappingmarkup 
     end

srv=net.createServer(net.TCP)
srv:listen(port,
     function(conn)
          conn:send("HTTP/1.1 200 OK\nContent-Type: text/html\nRefresh: 5\n\n" ..
              "<!DOCTYPE HTML>" ..
              "<html><body>" ..
              "<h2>Clixx.io Simple Thermo-RGB-LED Controller</h2></br>" ..
              "Temperature : " .. ds18b20.read() .. "<br>" ..
              "Node ChipID : " .. node.chipid() .. "<br>" ..
              "Node MAC : " .. wifi.sta.getmac() .. "<br>" ..
              "Node Heap : " .. node.heap() .. "<br>" ..
              "Timer Ticks : " .. tmr.now() .. "<br>" ..
              statusReportHtml() ..
              "</html></body>")          
          conn:on("sent",function(conn) conn:close() end)
     end
)

-- This timer loop displays a colour by it's index in the temperature table
tmr.alarm(0,1000,1,function()
  local tempindex = 1;
  local t = ds18b20.read();
  local ci = 0
  if (t == nil) then
     -- If there's no temp sensor, try setting it up again for next time
     ds18b20.setup(gpio_0i)
     return end
     
  for k,v in pairs(tempmappings) do
      if t < v[1] then
          ci = v[2]
          ws2812.writergb(gpio_0o, string.char(colorvals[ci][1],colorvals[ci][2],colorvals[ci][3]));
          coloridx = ci
          return end
  end
  
end)
