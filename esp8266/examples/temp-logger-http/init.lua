-- Configure Gpio
sda = 4
scl = 3

-- Show a User Message
print("Welcome to the clixx.io Temperature Logger")

-- Import the lm75 module
require('lm75')
require('inifile')

-- Initialise
lm75:init(sda, scl)
print("Temp="..lm75:readTemp())
print('interavl', interval)

-- Connection information
config = inifile.parse('config.ini')
interval = (config['Transmission']['interval']) * 1000
serverport = config['Transmission']['serverport']
serverip = config['Transmission']['serverip']
projectname = config['Project']['name']

-- Callback Function to transmit the temperature
function log_temperature()
temp=lm75:readTemp()
sk=net.createConnection(net.TCP, 0)
sk:on("receive", function(sck, c) print(c) end )
sk:on("connection", function(sck) 
      sk:send("GET /logsensor/"..projectname.."?temp="..temp.." HTTP/1.0")
      tmr.delay(10000)
      print('.')
      sk:close()
      end )
sk:connect(serverport,serverip)
tmr.delay(10000)
end 

-- Start a Timer with a periodic Callback
tmr.alarm(1, interval * 1000, 1, log_temperature)
