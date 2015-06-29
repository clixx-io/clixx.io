-- Send a Packet on a Socket with keypress

count = 0
delay = 0
gpio.mode(4,gpio.INT)

function transmit_msg()
   sk=net.createConnection(net.TCP, 0)
   sk:on("receive", function(sck, c) print(c) end )
   sk:on("connection", function(sck) sk:send("ButtonPress\r\n") end )
   sk:connect(9000,"192.168.0.6")
   tmr.delay(1000000)
   print("Sent")
   end

function buttonpress(level)
   x = tmr.now()
   if x > delay then
      delay = tmr.now()+100000
      count = count + 1
      print(".")
      transmit_msg()
      end
   end

gpio.trig(4, "down",buttonpress)
