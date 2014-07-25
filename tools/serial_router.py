#!/usr/bin/env python

# Based on code by dpslwk 24/06/2013

import mosquitto
import sys, time, Queue, os
import serial, threading
import argparse

class SerialToMosquitto:

    MQTTServer = "test.mosquitto.org"
    clientName = "Clixx.io Router"

    def __init__(self,portname,baudrate,topic_pub,topic_sub):

        self.port = portname
        self.topic_pub = topic_pub
        self.topic_sub = topic_sub
        
        self.linebuffer = ""
        self.last_msg = ""

        self.queue = Queue.Queue()
        self.s =  serial.Serial()
        self.t = threading.Thread(target=self.run)

        # open serial port
        self.s.baudrate = baudrate
        self.s.timeout = 0
        self.serial_connect()
        
        # mqtt setup on
        self.client = mosquitto.Mosquitto(self.clientName)
        self.client.on_connect = self.on_connect
        self.client.on_disconnect = self.on_disconnect
        self.client.on_message = self.on_message

        print "Connecting.." , self.client.connect(self.MQTTServer,1883, 60, True)
        if len(self.topic_sub) != 0:
            print "Subscribing..", self.client.subscribe(self.topic_sub)

    def __del__(self):

        self.disconnect_all()

    def serial_connect(self):

        if self.s.isOpen() == False:
            self.s.port = self.port
            try:

                self.s.open()

                print "Connected"

            except serial.SerialException, e:

                sys.stderr.write("could not open port %r: %s\n" % (self.port, e))
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

            while True:
                while rc == 0:

                    # mqtt stuff
                    rc = self.client.loop()
                    # serial stuff
                    self.decodeLLAP()

                # we lost the network to mqtt
                print("We lost MQTT")
                
                time.delay(15)
                
                self.client.connect(self.MQTTServer,1883, 60, True)

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
            self.client.publish(self.topic_pub, msg)
            self.queue.task_done()

            print "Publishing msg", msg
            
        return True


if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='clixx.io Serial Router')
    parser.add_argument("portname", type=str,
                        help="Serial port file name")
    parser.add_argument("baudrate", type=int,
                        help="Baudrate setting")
                    
    parser.add_argument("publish_topic", type=str, help="Topic to publish on. Characters received on the Serial Stream are written out on this topic.", default="clixx.io/hello")    
    parser.add_argument("subscribe_topic", type=str, help="Topic to subscribe to. MQTT messages received are written to the serial port.", default=None)    
    parser.add_argument("-v", "--verbose", help="increase output verbosity",
                    action="store_true")
   
    args = parser.parse_args()
    
    if not os.path.exists(args.portname):
        print ("Port doesnt exist %s" % args.portname)
        sys.exit(0)
        
    if not args.baudrate in [2400,9600,19200,57600,115200]:
        print ("Baudrate is not supported %s" % args.baudrate)
        sys.exit(0)

    application = SerialToMosquitto(portname,baudrate,args.pub,args.sub)

    application.main()
