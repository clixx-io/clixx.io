#!/usr/bin/python 

# Echo server program
import socket
import RPi.GPIO as GPIO

#####################################################################
#### SETUP IOs ######################################################
#####################################################################
# to use Raspberry Pi board pin numbers
GPIO.setmode(GPIO.BCM)
# For LED1 we use pin 4 according BCM pin count 
# (see https://projects.drogon.net/raspberry-pi/wiringpi/pins/)
LED1 = 4
# For Switch input we use pin 17 according BCM pin count
SWITCH1 = 17
# set up GPIO output channel
GPIO.setup(LED1, GPIO.OUT)

# set up GPIO input with pull-up control
#   (pull_up_down be PUD_OFF, PUD_UP or PUD_DOWN, default PUD_OFF)
GPIO.setup(SWITCH1, GPIO.IN, pull_up_down=GPIO.PUD_UP)


#####################################################################
#### SETUP SOCKET INTERFASE #########################################
#####################################################################
HOST = ''                 # Symbolic name meaning the local host
PORT = 54321              # Arbitrary non-privileged port
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST, PORT))
s.listen(1)
conn, addr = s.accept()
print 'Connected by', addr
#####################################################################
#### Continous loop, that waits for new data and acts depending  ####
#### on data content on it                                       ####
#####################################################################
while 1:
    data = conn.recv(1024)
    print 'New Receive', data
    if not data: break
    print 'Received Data:', data
    if data == 'LED1 on\n':
        conn.send('on')
        GPIO.output(LED1, GPIO.HIGH)
        print 'received on'
    elif data == 'LED1 off\n':
        conn.send('off')
        GPIO.output(LED1, GPIO.LOW)
        print 'received off'
    elif data == 'get status\n':
        print 'Input Status:', GPIO.input(SWITCH1)
        if GPIO.input(SWITCH1):
           conn.send('on')  
           print 'Read GIOP 0 result On'
        else:
           conn.send('off')  
           print 'Read GIOP 0 result Off'
        # ende if
    else:
        conn.send('unknown command')
        print 'Unknown command:', data
    # continue with while 1

conn.close()
