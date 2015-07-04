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
print(lm75:readTemp())

function log_temperature()
sk=net.createConnection(net.TCP, 0)
sk:on("receive", function(sck, c) print(c) end )
sk:connect(sport, sip)
print('sending...')
sk:send("GET /logsensor/"..project.."?temp="..lm75:readTemp().." HTTP/1.1 \r\n\r\n")
end

config = inifile.parse('config.ini')
interval = (config['Transmission']['interval']) * 1000
sport = config['Transmission']['serverport']
sip = config['Transmission']['serverip']
project = config['Project']['name']

tmr.alarm(1, interval, 1, log_temperature)
--tmr.alarm(2, 5000, 1, connect_thread)
