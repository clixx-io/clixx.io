#!/usr/bin/env python

# Based on code by dpslwk 24/06/2013

import mosquitto
import sys, time, Queue
import serial, threading

class MQTTLAPP:

    # subscribe to Clixx.io topic
    s_tx = "clixx.io/hello"

    # publish from Clixx.io topic
    p_rx = "clixx.io/from"

    # serial port for URF
    port = "/dev/ttyUSB1"

    MQTTServer = "test.mosquitto.org"
    clientName = "Clixx.io Router"

    def __init__(self):

        self.queue = Queue.Queue()
        self.s =  serial.Serial()
        self.t = threading.Thread(target=self.run)

        # open serial port
        self.s.baudrate = 9600
        self.s.timeout = 0
        self.connect()
        
        self.linebuffer = ""
        self.last_msg = ""

        # mqtt setup on
        self.client = mosquitto.Mosquitto(self.clientName)
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        self.client.on_message = self.on_message

        print "Connecting.." , self.client.connect(self.MQTTServer,1883, 60, True)
        # self.client.subscribe(self.s_tx)

    def __del__(self):

        self.disconnect_all()

    def connect(self):

        print "Connected"

        if self.s.isOpen() == False:
            self.s.port = self.port
            try:

                self.s.open()

            except serial.SerialException, e:

                sys.stderr.write("could not open port %r: %s\n" % (port, e))
                sys.exit(1)

            # start the read thread
            self.t.start()
            return True

        else:

            return False

    def disconnect(self):

        # closeing the serial port will stop the thread
        if self.s.isOpen() == True:
            self.s.close()

    def run(self):

        while self.s.isOpen() == True:

            if self.s.inWaiting():

                newchars = self.s.read()
                
                # Add newly received characters to the linebuffer
                self.linebuffer += newchars
                
                if ('\n' in self.linebuffer):
                    n = self.linebuffer.index('\n')
                    msg = self.linebuffer[:n]
                    self.linebuffer = self.linebuffer[n+1:]

                    if msg != self.last_msg:
                        self.queue.put(msg)
                        self.last_msg = msg
                        print(msg)


    def sendLLAP(self, llapMsg):

        while len(llapMsg) < 12:

            llapMsg += '-'

        if self.s.isOpen() == True:

            self.s.write(llapMsg)


    def main(self):

        # loop

        rc = 0

        try:

            while rc == 0:

                # mqtt stuff
                rc = self.client.loop()
                # serial stuff
                self.decodeLLAP()

            # we lost the network to mqtt
            print("We lost MQTT")

        except KeyboardInterrupt:

            print("Keyboard Quit")
            self.disconnect_all()

    def disconnect_all(self):

        self.client.unsubscribe(self.s_tx)
        self.client.loop()
        self.client.disconnect()
        self.client.loop()
        self.disconnect()

    def on_connect(self, mosq, obj, rc):

        if rc == 0:
            print("Connected successfully.")
        else:
            print("Error %d connecting " % rc)
            print mosq, obj

    def on_disconnect(self, mosq, obj, rc):

        if rc == 1:

            # unexpected disconnect, reconnect?
            pass

        else:

            # expected disconnect
            print("Disconnected successfully.")

    def on_message(self, mosq, obj, msg):

        # got mqtt message to send
        print("Message received on topic "+msg.topic+" with QoS "+str(msg.qos)+" and payload "+msg.payload)

        # push to serial
        self.s.write(msg.payload)


    def decodeLLAP(self):

        if not self.queue.empty():

            msg = self.queue.get()

            # publish to mqtt
            self.client.publish(self.s_tx, msg)
            self.queue.task_done()

            print "Publishing msg", msg
            
        return True


if __name__ == "__main__":

    application = MQTTLAPP()

    application.main()
