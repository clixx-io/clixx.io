--Configure Gpio
d1_output = 3
d1_input  = 4

--Show a User Message
print("Welcome to the clixx.io Web PWM Controller")

-- set gpio 0 as output.
gpio.mode(d1_output, gpio.OUTPUT)
gpio.write(d1_output,gpio.LOW)
gpio.mode(d1_input, gpio.INPUT)

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
        buf = buf.."<h1>Clixx.io ESP8266 Web PWM Device</h1>";
        buf = buf.."<form src=\"/\">";
        buf = buf.."PWM Value (0-1023):<br>";
        buf = buf.."<input type=\"text\" name=\"pwmvalue\" value=\"1023\"><br>";
--      buf = buf.."Time to run (Seconds):<br>";
--      buf = buf.."<input type=\"text\" name=\"runtime\"><br>";
        buf = buf.."<br>Turn Digital Output Pin (gpio4) <select name=\"pin\" onchange=\"form.submit()\">";
        local _on,_off = "",""
        if(_GET.pin == "ON")then
              _on = " selected=true";
              if (_GET.pwmvalue == "") then
                  gpio.mode(d1_output, gpio.OUTPUT)
                  gpio.write(d1_output,gpio.HIGH)
              else    
                  pwm.setup(d1_output, 1000, tonumber(_GET.pwmvalue))
                  pwm.start(d1_output)
                  end
        elseif(_GET.pin == "OFF")then
              _off = " selected=\"false\"";
              pwm.close(d1_output)
              
              gpio.mode(d1_output, gpio.OUTPUT)
              gpio.write(d1_output,gpio.LOW)
        end
        buf = buf.."<option".._off..">OFF</opton><option".._on..">ON</option></select><br><br>";
        buf = buf.."</form>";
        client:send(buf);
        client:close();
        collectgarbage();
    end)
end)
