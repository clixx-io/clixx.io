---
-- @description Provides a simple web server to display temperature via a web page
-- MCP23008 Datasheet: http://ww1.microchip.com/downloads/en/DeviceDoc/21919e.pdf
-- Tested on NodeMCU 0.9.5 build 20150213.
-- @date March 02, 2015
-- @author David 
--  GitHub: https://github.com/clixx-io 
--  Website: http://www.clixx.io
--------------------------------------------------------------------------------

-- Configure Gpio
sda = 4
scl = 3

-- Show a User Message
print("Welcome to the clixx.io Temperature Server")

-- Import the lm75 module
require('lm75')

-- Initialise
lm75:init(sda, scl)
print(lm75:readTemp())

-- Create a simple web server
srv=net.createServer(net.TCP) 
srv:listen(80,function(conn) 
    conn:on("receive", function(client,request)
        local buf = "";
        local _, _, method, path, vars = string.find(request, "([A-Z]+) (.+)?(.+) HTTP");
        if(method == nil)then 
            _, _, method, path = string.find(request, "([A-Z]+) (.+) HTTP"); 
        end
        local _GET = {}
        if (vars ~= nil)then 
            for k, v in string.gmatch(vars, "(%w+)=(%w+)&*") do 
                _GET[k] = v; 
                print(k,'=',v);
            end 
        end
        buf = buf.."<h1>ESP8266 IoT Device</h1>";
        buf = buf.."<form src=\"/\">";
        buf = buf.."Temperature is (degrees C):<br>";
        buf = buf.."<h2>"..lm75:readTemp().."</h2>";
        buf = buf.."<br>";
        buf = buf.."</form>";
        client:send(buf);
        client:close();
        collectgarbage();
    end)
end)
