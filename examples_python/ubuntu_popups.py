#!/usr/bin/python

# http://oliversmith.io/technology/2010/08/15/first-steps-using-python-and-mqtt/

# http://nthn.me/posts/2012/mqtt.html
# https://github.com/njh/NanodeMQTT/tree/master/examples
# https://github.com/tobyjaffey/mqtt-serial-router/blob/master/mqtt.c
# http://knolleary.net/arduino-client-for-mqtt/api/
#http://oliversmith.io/technology/2010/08/15/first-steps-using-python-and-mqtt/

import pynotify
import mosquitto

#define what happens after connection
def on_connect(rc):
    print "Connected"

#On recipt of a message create a pynotification and show it
def on_message(msg):
    n = pynotify.Notification (msg.topic, msg.payload)
    n.show ()

pynotify.init("python_sub")

#create a broker
mqttc = mosquitto.Mosquitto("python_sub")

#define the callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect

#connect
mqttc.connect("localhost", 1883, 60, True)

#subscribe to topic test
mqttc.subscribe("hello/world", 2)


#keep connected to broker
while mqttc.loop() == 0:
    pass
