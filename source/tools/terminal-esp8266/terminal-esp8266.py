#!/usr/bin/python
# -*- coding: utf-8 -*-

import os, sys, platform, glob
import serial
from PySide import QtGui, QtCore
from PySide.QtCore import *
from PySide.QtGui import *
from serialevents import SerialEvents
from pyterm_exceptions import *
import time

verbose = 1

        
def main(args):
	
	def quit():
		se.stop()
		s.close()
	
	def portChanged(portName):
		print('Port changed: ', portName)
		
	def tryConnect():
		if not s.isOpen():
			s.port = ports_cb.currentText()
			s.open()
			connect.setText('Connected')

	def readChipID():
		s.write("print(node.chipid())\n")

	def showip():
		s.write('print(wifi.sta.getip())\n')

	def list_accesspoints():
		""" This function will print out the Access-Point list """
		listap = ["function listap(t)\n",
                  "  for k,v in pairs(t) do\n",
                  "    print(k..\" : \"..v)\n",
                  "  end\n",
                  "end\n",
                  "wifi.sta.getap(listap)\n"]
                  
		for c in listap:
			s.write(c)    
    
	def setwifi():
		s.write('wifi.setmode(wifi.STATION)\n')
		s.write('wifi.sta.config("%s", "%s")\n' % (str(ssidtext.text()), str(pwdtext.text())))
		s.write('wifi.sta.connect()\n')
		#print 'status\n', se.readAll().decode("utf-8")

	def sendStr():
		t = str(sendtext.toPlainText()+'\n')
		print "Text=",t
		if len(t)>0:
			s.write(t)
	def write():
		#print 'NEW DATA?',se.readAll().decode("utf-8")
		textedit.setText(textedit.toPlainText() + se.readAll().decode("utf-8"))
		return

	def restart():
		textedit.setText("")
		s.write('node.restart()\n')
	
	# A function that tries to list serial ports on most common platforms
	def list_serial_ports():
		system_name = platform.system()
		if system_name == "Windows":
			# Scan for available ports.
			available = []
			for i in range(256):
				try:
					s = serial.Serial(i)
					available.append("COM%d" % (i+1))
					s.close()
				except serial.SerialException:
					pass
			return available
		elif system_name == "Darwin":
			# Mac
			return glob.glob('/dev/tty*') + glob.glob('/dev/cu*')
		else:
			# Assume Linux or something else
			return glob.glob('/dev/ttyS*') + glob.glob('/dev/ttyUSB*')

	a = QtGui.QApplication(args)
	
	matchedPorts = list_serial_ports()
	
	ports_cb = QtGui.QComboBox()
	ports_cb.currentIndexChanged[str].connect(portChanged)
	
	for port in reversed(matchedPorts):
		ports_cb.addItem(port)
	
	connect = QPushButton("Connect")
	connect.clicked.connect(tryConnect)

	uart = QHBoxLayout()
	uart.addWidget(ports_cb)
	uart.addWidget(connect)

	#chipid = QPushButton("Read chipID")
	#chipid.clicked.connect(readChipID)
	wifi = QHBoxLayout()
	wifi1 = QVBoxLayout()
	ssidlabel = QLabel('SSID:')
	ssidlabel.setAlignment(Qt.AlignLeft)
	ssidtext = QLineEdit()
	ssidtext.setMaxLength(32)
	ssidtext.setMaximumWidth(80)
	ssidtext.setAlignment(Qt.AlignLeft)
	wifi1.addWidget(ssidlabel)
	wifi1.addWidget(ssidtext)
	wifi2 = QVBoxLayout()
	pwdlabel = QLabel('Password:')
	pwdlabel.setAlignment(Qt.AlignLeft)
	pwdtext = QLineEdit()
	pwdtext.setMaxLength(32)
	pwdtext.setMaximumWidth(80)
	pwdtext.setEchoMode(QLineEdit.Password)
	pwdtext.setAlignment(Qt.AlignLeft)
	wifi2.addWidget(pwdlabel)
	wifi2.addWidget(pwdtext)
	wifi3 = QVBoxLayout()
	setupwifi = QPushButton("Setup Wifi")
	setupwifi.clicked.connect(setwifi)
	ip = QPushButton('Show IP')
	ip.clicked.connect(showip)
	wifi3.addWidget(ip)
	wifi3.addWidget(setupwifi)
	wifi.addLayout(wifi1)
	#wifi.addStretch(1)
	wifi.addLayout(wifi2)
	wifi.addLayout(wifi3)

	comm = QVBoxLayout()
	textedit = QTextEdit()
	sendlabel = QLabel('Send Text')
	sendtext = QTextEdit()
	sendtext.setMaximumHeight(sendlabel.sizeHint().height() * 4)
	send = QPushButton("Send")
	send.clicked.connect(sendStr)

	reset = QPushButton("Restart")
	reset.clicked.connect(restart)

	comm.addWidget(textedit)
	comm.addWidget(sendlabel)
	comm.addWidget(sendtext)
	comm.addWidget(send)

	comm.addWidget(reset)

	full = QVBoxLayout()
	full.addLayout(uart)
	full.addLayout(wifi)
	full.addLayout(comm)

	s = serial.Serial()
	
	se = SerialEvents(s)
	se.readyRead.connect(write)
	se.start()
	
	window =  QWidget()
	layout =  QVBoxLayout()
	
	window.setLayout(full)
	window.setWindowTitle('Terminal for ESP8266')
	window.show()
	
	r = a.exec_()
	quit()
	return r

if __name__ == "__main__":

	main(sys.argv)

