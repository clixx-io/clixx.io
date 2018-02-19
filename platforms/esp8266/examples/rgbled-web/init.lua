----------------------------------------------------------------
--
-- Clixx.IO 
--
-- Change Colour on a WS2812 RGBLED via Web Page
--
-- (c) 2016 David Lyon
--
----------------------------------------------------------------

port = 80

gpio_0i, gpio_0o = 4,3        -- ESP-01 GPIO Mapping
d1_output=3

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


print("clixx.io RGB Web LED")

srv=net.createServer(net.TCP)
srv:listen(port,
     function(conn)
         local filename = "base.html"
         if file.open(filename, "r") then
                 local line=file.read(128)
                 if line then 
                     conn:send(line) 
                 end
         end
           
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

             local r,g,b = 50,0,20
             if _GET.r ~= "" then r = tonumber(_GET.r) end
             if _GET.g ~= "" then g = tonumber(_GET.g) end
             if _GET.b ~= "" then b = tonumber(_GET.b) end
             
             buf = buf.."<h1>Clixx.io ESP8266 RGB-LED Device</h1>";
             buf = buf.."<form src=\"/\">";
             buf = buf.."PWM Value (0-1023):<br>";
             buf = buf.."<input type=\"text\" name=\"r\" value=\"" .. r .. "\"><br>";
             buf = buf.."<input type=\"text\" name=\"g\" value=\"" .. g .. "\"><br>";
             buf = buf.."<input type=\"text\" name=\"b\" value=\"" .. b .. "\"><br>";
             buf = buf.."<br>Turn RGB-LED-1 (gpio3) <select name=\"pin\" onchange=\"form.submit()\">";
             local _on,_off = "",""
             
             if(_GET.pin == "ON")then
             
                  ws2812.writergb(3, string.char(r,g,b))

                  _on = " selected=true";

             elseif(_GET.pin == "OFF")then
                   _off = " selected=\"false\"";
                  ws2812.writergb(3, string.char(0,0,0),string.char(0,0,0))
                  
             end
             buf = buf.."<option".._off..">OFF</opton><option".._on..">ON</option></select><br><br>";
             buf = buf.."</form>";
             client:send(buf);
             client:close();
             collectgarbage();
         end)

     end
)

