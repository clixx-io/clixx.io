MQTT_HOST = "192.168.0.2"     -- Change for your setup
MQTT_PORT = 1883              -- Change for your setup
MQTT_CLIENTID = "ESP8266"     -- Change for your setup

-- initialize mqtt client with keepalive timer of 60sec
m = mqtt.Client(MQTT_CLIENTID, 60, "", "") -- Living dangerously. No password!


-- Set up Last Will and Testament (optional)
-- Broker will publish a message with qos = 0, retain = 0, data = "offline"
-- to topic "/lwt" if client don't send keepalive packet
m:lwt("/lwt", "Oh noes! Plz! I don't wanna die!", 0, 0)


-- When client connects, print status message and subscribe to cmd topic
m:on("connect", function(m) 
     -- Serial status message
     print ("\n\n", MQTT_CLIENTID, " connected to MQTT host ", MQTT_HOST,
          " on port ", MQTT_PORT, "\n\n")

     -- Subscribe to the topic where the ESP8266 will get commands from
     m:subscribe("/rgbled/cmd/#", 0,
          function(m) print("Subscribed to CMD Topic") end)
end)


-- When client disconnects, print a message and list space left on stack
m:on("offline", function(m)
     print ("\n\nDisconnected from broker")
     print("Heap: ", node.heap())
end)


-- On a publish message receive event, run the message dispatcher and
-- interpret the command
m:on("message", function(m,t,pl)
     print("PAYLOAD: ", pl)
     print("TOPIC: ", t)

     -- Use the built-in json decoder to extract the values
     local jsn = cjson.decode(pl)

     -- Write each of the elements to the RGB-LED
     ws2812.writergb(3, string.char(jsn.r,jsn.g,jsn.b))
     
end)

tmr.alarm(1, 1500, 1, function()
  if wifi.sta.getip()== nil then
  print("IP unavaiable, Waiting...")
  -- Turn On the LED to show the system is operational
  ws2812.writergb(3, string.char(20,180,20))
 else
    tmr.stop(1)
    print("ESP8266 mode is: " .. wifi.getmode())
    print("The module MAC address is: " .. wifi.ap.getmac())
    print("Config done, IP is "..wifi.sta.getip())
    -- Connect to the broker
    m:connect(MQTT_HOST, MQTT_PORT, 0, 1)
    ws2812.writergb(3, string.char(20,180,180))
 end
 end)
