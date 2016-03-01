----------------------------------------------------------------
--
-- Clixx.IO 
--
-- Drive a DC-Motor based based on temperature
--
-- (c) 2016 David Lyon
--
----------------------------------------------------------------

require('ds18b20')

print("clixx.io Simple Thermo-Motor Controller")

port = 80

gpio_0i, gpio_0o = 4,3   -- ESP-01 GPIO Mapping
orv = 0                  -- PWM Pin Output value

ds18b20.setup(gpio_0i)

pwm.setup(gpio_0o, 500, 512)
pwm.start(gpio_0o)

-- A lookup table for temperature threshold to PWM value
tempmappings = {
             {0,0},
             {10,200},         
             {15,300},
             {20,400},         
             {25,500},     
             {30,600},     
             {35,800},     
             {40,1023},     
     }            

function statusReportHtml()
     -- Generate a Table showing the current temperature thresholds
     local mappingmarkup
     
     mappingmarkup = "<br/><table border=\"1\" style=\"width:35%\"><caption>Temperature Run Settings</caption>"
     mappingmarkup = mappingmarkup .. "<tr><th>Temperature</th><th>PWM Value 0-1023</th></tr>"
     for k,v in pairs(tempmappings) do
          if (v[2] == orv) then
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
              "<h2>clixx.io Simple Thermo-Motor Controller</h2></br>" ..
              "Temperature : " .. ds18b20.read() .. "<br>" ..
              "Current PWM Run Value : " .. orv .. "<br>" ..
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
  if (t == nil) then
     -- If there's no temp sensor, try setting it up again for next time
     ds18b20.setup(gpio_0i)
     return end
     
     -- Convert a temperature to a PWM value using the lookup table
     for k,v in pairs(tempmappings) do
           if t < v[1] then
               orv = v[2]
               pwm.setduty(gpio_0o, orv)
               return end
       end
  
end)
