print('Welcome to temperature logger...')
--wifi.setmode(wifi.STATION)
--wifi.sta.config("sufi", "abbas123")
--wifi.sta.connect()

count = 0

function log_temperature()
sk=net.createConnection(net.TCP, 0)
sk:on("receive", function(sck, c) print(c) end )
sk:connect(5000,"192.168.43.94")
print('sending...')
sk:send("GET /logsensor/frank?temp="..count.." HTTP/1.1 \\r\\n\\r\\n")
count = count + 1
end

tmr.alarm(1, 10000, 1, log_temperature)
--tmr.alarm(2, 5000, 1, connect_thread)
