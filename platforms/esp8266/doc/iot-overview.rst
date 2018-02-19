IoT and Networking Overview
===========================

Introduction
------------

The ESP-8266 are very powerful devices and have many the ability
to be easily networked to provide practical solutions. By interconnecting
different devices you can increase the usefulness of your network.


.. image:: network-iot-overview.png
   :scale: 50 %

There are quite a number of different ways that the Devices can connect and
talk to each other on the network and shown above is a simplified network 
map.

The different communication methods are shown in the table and are used in
different applications. There's really no ideal communication protocol.

TCP
---

TCP or Transmission-Control-Protocol is a stream oriented protocol for
sending data over a network. Practically every programming language these
days will provide support for TCP-Sockets and it's definitely available
in the NodeMCU software.

TCP is the base protocol for most of the higher level protocols like HTTP
that is the base protocol for the World-Wide-Web.

UDP
---

UDP is a connectionless protocol where delivery of packets isn't checked
or garuenteed. Because of this it's faster than TCP but perhaps slightly
less reliable.

HTTP
----

Http is the basic protocol for the World-Wibe-Web. You can also use it
on the ESP8266 to send commands to other machines or receive commands
in HTTP format and respond to them.

MQTT
---- 

MQTT is a relatively new protocol for M2M or Machine-To-Machine communications.
