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

port = 80

-- ESP-01 GPIO Mapping
gpio_0i, gpio_0o = 4,3


orv = 0
mappingmarkup = ""

pwm.setup(gpio_0o, 500, 512)
pwm.start(gpio_0o)

tempmappings = {
             -- A temperature threshold then a PWM value for the output port
             {0,0},
             {10,200},         
             {15,300},
             {20,400},         
             {25,500},     
             {30,600},     
             {35,800},     
             {40,1023},     
     }            

-- Generate a Table showing the current temperature thresholds
mappingmarkup = "<br/><table border=\"1\" style=\"width:35%\"><caption>Temperature Run Settings</caption>"
mappingmarkup = mappingmarkup .. "<tr><th>Temperature</th><th>PWM Value 0-1023</th></tr>"
for k,v in pairs(tempmappings) do
     mappingmarkup = mappingmarkup .. "<tr>" .. "<td>" .. v[1] .. "</td><td>" .. v[2] .. "</td></tr>"
     end
mappingmarkup = mappingmarkup .. "</table>"

ds18b20.setup(gpio_0i)

print("clixx.io Temperature Controlled Motor")

srv=net.createServer(net.TCP)
srv:listen(port,
     function(conn)
          conn:send("HTTP/1.1 200 OK\nContent-Type: text/html\nRefresh: 5\n\n" ..
              "<!DOCTYPE HTML>" ..
              "<html><body>" ..
              "<h2>Clixx.io Temperature to PWM Controller</h2></br>" ..
              "Temperature : " .. ds18b20.read() .. "<br>" ..
              "Current PWM Run Value : " .. orv .. "<br>" ..
              "Node MAC : " .. wifi.sta.getmac() .. "<br>" ..
              "Node Heap : " .. node.heap() .. "<br>" ..
              "Timer Ticks : " .. tmr.now() .. "<br>" ..
              mappingmarkup ..
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
     
  for k,v in pairs(tempmappings) do
      if t < v[1] then
          orv = v[2]
          pwm.setduty(gpio_0o, orv)
          return end
  end
  
end)
