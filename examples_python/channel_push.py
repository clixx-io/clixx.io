import platform
import mosquitto

#create a node based on our machine name
iot = mosquitto.Mosquitto(platform.node()+'-')

# mqttc.will_set("/event/dropped", "Sorry, I seem to have died.")

#connect
iot.connect("test.mosquitto.org", 1883, 60, True)

iot.publish("clixx.io/hello", "Hello, from %s!" % platform.node())
