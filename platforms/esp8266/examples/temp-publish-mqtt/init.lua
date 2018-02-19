-- =====================================================================================================

-- NodeMCU Temperature Publishing to MQTT Example

-- (c) 2016 David Lyon

-- =====================================================================================================

--  This software is licensed under the Creative Attribution 3.0 Licence Agreement
--
--    You are free to:
--
--        Share — copy and redistribute the material in any medium or format
--        Adapt — remix, transform, and build upon the material
--        for any purpose, even commercially.
--
--        The licensor cannot revoke these freedoms as long as you follow the license terms.
--
--      Under the following terms:
--        Attribution — You must give appropriate credit, provide a link to the license, and indicate if changes were made. You may do so in any reasonable manner, but not in any way that suggests the licensor endorses you or your use.
--        No additional restrictions — You may not apply legal terms or technological measures that legally restrict others from doing anything the license permits.
--

require('ds18b20')

-- Configuration to connect to the MQTT broker.
BROKER = "test.mosquitto.org"               -- Ip/hostname of MQTT broker
BRPORT = 1883                               -- MQTT broker port
BRUSER = ""                                 -- If MQTT authenitcation is used then define the user
BRPWD  = ""                                 -- The above user password
CLIENTID = "ESP8266-" ..  node.chipid()     -- The MQTT ID. Change to something you like
TXINTVLMS = 10000                           -- Transmission interval in Miliseconds
COLOR_NOWIFI = string.char(200,10,10)
COLOR_CONNECTBROKER = string.char(20,120,80)
COLOR_ONLINE = string.char(0,220,220)
MQTT_CHANNEL = "/clixx.io/temperature"      -- Channel Topic

-- Control variables.
gpio_0i, gpio_0o = 4,3                      -- ESP-01 GPIO Mapping

-- Initialise the Temperature Sensor
ds18b20.setup(gpio_0i)

-- Create an MQTT Object
m = mqtt.Client( CLIENTID, 120, BRUSER, BRPWD)
MQTT_CHANNEL = MQTT_CHANNEL .. '/' .. CLIENTID

-- When client disconnects, print a message and list space left on stack
m:on("offline", function(m)
     ws2812.writergb(3, COLOR_CONNECTBROKER)
     print ("\n\nDisconnected from broker")
     print("Heap: ", node.heap())
     -- Reset the unit and try to reconnect
     node.reset()
end)


tmr.alarm(1, 1500, 1, function()
    if wifi.sta.getip()== nil then
        print("IP unavaiable, Waiting...")
        -- Turn On the LED to show the system is operational
        ws2812.writergb(3, COLOR_NOWIFI)
    else
        tmr.stop(1)
        print("ESP8266 mode is: " .. wifi.getmode())
        print("The module MAC address is: " .. wifi.ap.getmac())
        print("Config done, IP is "..wifi.sta.getip())
        -- Connect to the broker
        ws2812.writergb(3, COLOR_CONNECTBROKER)
     
        print "Connecting to MQTT broker. Please wait..."
        m:connect( BROKER , BRPORT, 0, function(conn)
            ws2812.writergb(3, COLOR_ONLINE)
            print("Connected to MQTT:" .. BROKER .. ":" .. BRPORT .." as " .. CLIENTID )
            tmr.alarm(2, TXINTVLMS, 1, function()
                local d = ""
                local t = ds18b20.read()

                ok, json = pcall(cjson.encode, {temp_c=t})
                if ok then
                  d = json
                  m:publish(MQTT_CHANNEL,d,0,0, function(conn) 
                    -- Callback function. We've sent the data
                    print("Data sent: " .. d)
                  end)
                else
                  print("failed to encode!")
                end
            end)
        end)
     end
 end)

