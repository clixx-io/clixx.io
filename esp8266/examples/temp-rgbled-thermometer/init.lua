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
             {40,1024},     
     }            

ds18b20.setup(gpio_0i)

print("clixx.io Temperature Controlled Motor")

srv=net.createServer(net.TCP)
srv:listen(port,
     function(conn)
          conn:send("HTTP/1.1 200 OK\nContent-Type: text/html\nRefresh: 5\n\n" ..
              "<!DOCTYPE HTML>" ..
              "<html><body>" ..
              "<b>ESP8266</b></br>" ..
              "Temperature : " .. ds18b20.read() .. "<br>" ..
              "Node ChipID : " .. node.chipid() .. "<br>" ..
              "Node MAC : " .. wifi.sta.getmac() .. "<br>" ..
              "Node Heap : " .. node.heap() .. "<br>" ..
              "Timer Ticks : " .. tmr.now() .. "<br>" ..
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
          print(v[2])
          pwm.setduty(gpio_0o, v[2])
          return end
  end
  
end)
