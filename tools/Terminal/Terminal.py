#!/usr/bin/python
# -*- coding: utf-8 -*-

import os, sys
import serial
from PySide import QtGui, QtCore
from PySide.QtCore import *
from PySide.QtGui import *
from portsListener import PortsListener
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
	
	def sendStr():
		t = str(sendtext.toPlainText()+'\n')
		print "Text=",t
		if len(t)>0:
			s.write(t)
		
	def write():
		textedit.setText(textedit.toPlainText() + se.readAll().decode("utf-8"))
		return
	
	a = QtGui.QApplication(args)
	
	#matchedPorts = PortsListener.getPorts()
	matchedPorts = ["COM4:"]
	
	ports_cb = QtGui.QComboBox()
	ports_cb.currentIndexChanged[str].connect(portChanged)
	
	for port in matchedPorts:
		ports_cb.addItem(port)	# TODO: add QVariant with port data
	
	connect = QPushButton("Connect")
	connect.clicked.connect(tryConnect)
	
	send = QPushButton("Send")
	send.clicked.connect(sendStr)
	
	textedit = QTextEdit()
	sendlabel = QLabel('Send Text')
	sendlabel.setAlignment(Qt.AlignLeft)
	sendtext = QTextEdit()
	sendtext.setMaximumHeight(sendlabel.sizeHint().height() * 4)
       	
	s = serial.Serial()
	
	se = SerialEvents(s)
	se.readyRead.connect(write)
	se.start()
	
	# try:
	# 	s = serial.Serial('/dev/cu.usbserial-A6008cB6')
	# except (FileNotFoundError, serial.SerialException):
	# 	print("Error")
	
	window =  QWidget()
	layout =  QVBoxLayout()
	
	layout.addWidget(ports_cb)
	layout.addWidget(connect)
	layout.addWidget(textedit)
	layout.addWidget(sendlabel)
	layout.addWidget(sendtext)
	layout.addWidget(send)
	
	window.setLayout(layout)
	window.show()
	
	r = a.exec_()
	quit()
	return r

if __name__ == "__main__":

	main(sys.argv)
	
