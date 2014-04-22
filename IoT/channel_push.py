import platform
import mosquitto

#create a broker using our machine name
mqttc = mosquitto.Mosquitto(platform.node()+'-')

# mqttc.will_set("/event/dropped", "Sorry, I seem to have died.")

#connect
mqttc.connect("test.mosquitto.org", 1883, 60, True)

mqttc.publish("clixx.io/hello", "Hello, from %s!" % platform.node())
