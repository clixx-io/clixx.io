#!/usr/bin/python

# http://oliversmith.io/technology/2010/08/15/first-steps-using-python-and-mqtt/

# http://nthn.me/posts/2012/mqtt.html
# https://github.com/njh/NanodeMQTT/tree/master/examples
# https://github.com/tobyjaffey/mqtt-serial-router/blob/master/mqtt.c
# http://knolleary.net/arduino-client-for-mqtt/api/
#http://oliversmith.io/technology/2010/08/15/first-steps-using-python-and-mqtt/

import pynotify
import mosquitto
import sys, platform
import clixxIO 

#define what happens after connection
def on_connect(rc):
    print("Connected to %s as %s" % ('test.mosquitto.org',platform.node()))

#define what happens if connection drops
def on_disconnect(rc):
    print("Disconnect")

#On recipt of a message create a pynotification and show it
def on_message(msg):
    if msg.topic == "clixx.io/hello":
        n = pynotify.Notification (msg.topic, msg.payload)
        n.show ()
    else:
        print "Received %s %s" % (msg.topic, msg.payload)

pynotify.init(platform.node())

#create a broker using our machine name
mqttc = mosquitto.Mosquitto(platform.node())

#define the callbacks
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_disconnect = on_disconnect

#connect
mqttc.connect("test.mosquitto.org", 1883, 60, True)

#subscribe to topic test
mqttc.subscribe("clixx.io/hello", 2)
for topic in clixxIO.clixxIOListAllProjectMqttSubs():
    print "Subscribing to %s" % topic
    mqttc.subscribe(topic, 2)

#keep connected to broker
while mqttc.loop() == 0:
    pass
