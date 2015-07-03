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
from time import sleep

verbose = 1
config_mode = 0

def main(args):
	def writeln(data):
		s.write(data)
		sleep(.3)
	def quit():
		se.stop()
		s.close()

	#def delay():
	#	for i in range(5):
	#		sleep(.1)
	def portChanged(portName):
		print('Port changed: ', portName)
		
	def tryConnect():
		if not s.isOpen():
			s.port = ports_cb.currentText()
			s.open()
			connect.setText('Connected')
	def restart():
		textedit.setText("")
		writeln('node.restart()\n')

	def readChipID():
		writeln("print(node.chipid())\n")

	def showip():
		writeln('print(wifi.sta.getip())\n')

	def list_accesspoints():
		""" This function will print out the Access-Point list """
		listap = ["function listap(t)\n",
                  "  for k,v in pairs(t) do\n",
                  "    print(k..\" : \"..v)\n",
                  "  end\n",
                  "end\n",
                  "print('***Wifi Networks:***')\n",
                  "wifi.sta.getap(listap)\n"]
		for c in listap:
			writeln(c)
	def setwifi():
		writeln('wifi.setmode(wifi.STATION)\n')
		writeln('wifi.sta.config("%s", "%s")\n' % (str(ssidtext.text()), str(pwdtext.text())))
		writeln('wifi.sta.connect()\n')

	def down():
		global config_mode
		config_mode=1
		textedit.setText("")
		writeln('file.open("%s")\n' % str(fname.text()))
		writeln('print(file.read())\n')
		writeln('file.close()\n')
		flabel.setText(fname.text())

	def uploadfile():
		fileName,_ = QFileDialog.getOpenFileName(upload, "Open File", QtCore.QDir.currentPath())
		if fileName:
			try:
				f = open(fileName, "rt")
			except:
				textedit.setText(textedit.toPlainText() + "\nCould not open input file \"%s\"\n" % fileName)
			fileonesp = fileName[fileName.rfind('/')+1:]
			for ln in f:
				if len(ln) > 230:
					textedit.setText(textedit.toPlainText() + "File \"%s\" contains a line with more than 240 "
						"characters. This exceeds the size of the serial buffer.\n"
						% fileName)
					f.close()
			f.seek(0)
			textedit.setText(textedit.toPlainText() + "\nStage 1. Deleting old file from flash memory\n")
			writeln("file.open(\"" + fileonesp + "\", \"w\")\n")
			writeln("file.close()\n")
			writeln("file.remove(\"" + fileonesp + "\")\n")

			writeln("file.open(\"" + fileonesp + "\", \"w+\")\n")
			line = f.readline()
			textedit.setText(textedit.toPlainText() + "\nStage 3. Start writing data to flash memory...\n")
			while line != '':
				writeln('file.writeline("%s")\n' % line.strip())
				line = f.readline()

			f.close()
			writeln("file.flush()\r")
			writeln("file.close()\r")
	def writef():
		flabel.setText(fname.text())
		writeln('file.open("%s", "w+")\n' % str(fname.text()))
		l = str(fedit.toPlainText())
		for line in l.splitlines():
			writeln('file.writeline("%s")\n' % line)
		writeln('file.close()\n')
	def sendStr():
		t = str(sendtext.toPlainText()+'\n')
		print "Text=",t
		if len(t)>0:
			s.write(t)
	def write():
		textedit.setText(textedit.toPlainText() + se.readAll().decode("utf-8"))
		global config_mode
		if config_mode == 1:
			cfg = textedit.toPlainText()
			ind2 = cfg.find("> file.close()")
			if ind2 > 0:
				ind1 = cfg.find("file.read())")
				fedit.setText(cfg[ind1:ind2])
				config_mode = 0
		return

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

	reset = QPushButton("Restart")
	reset.clicked.connect(restart)

	uart = QHBoxLayout()
	uart.addWidget(ports_cb)
	uart.addWidget(connect)
	uart.addWidget(reset)

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

	wifi4 = QVBoxLayout()
	chipid = QPushButton("Read chipID")
	chipid.clicked.connect(readChipID)
	listAP = QPushButton("List AP")
	listAP.clicked.connect(list_accesspoints)
	wifi4.addWidget(chipid)
	wifi4.addWidget(listAP)

	wifi.addLayout(wifi1)
	#wifi.addStretch(1)
	wifi.addLayout(wifi2)
	wifi.addLayout(wifi3)
	wifi.addLayout(wifi4)

	comm = QVBoxLayout()
	textedit = QTextEdit()
	sendlabel = QLabel('Send Text')
	sendtext = QTextEdit()
	sendtext.setMaximumHeight(sendlabel.sizeHint().height() * 4)
	send = QPushButton("Send")
	send.clicked.connect(sendStr)

	comm.addWidget(textedit)
	comm.addWidget(sendlabel)
	comm.addWidget(sendtext)
	comm.addWidget(send)

	upload = QPushButton("Upload File")
	upload.clicked.connect(uploadfile)

	rw = QHBoxLayout()
	flabel = QLabel('File:')
	fname = QLineEdit()
	fname.setMaxLength(32)
	fname.setMaximumWidth(80)
	fname.setAlignment(Qt.AlignLeft)

	dl = QPushButton("Download")
	dl.clicked.connect(down)
	#shwconf = QPushButton("Show Config")
	#shwconf.clicked.connect(showconf)
	wfile = QPushButton("Write Back")
	wfile.clicked.connect(writef)
	rw.addWidget(flabel)
	rw.addWidget(fname)
	rw.addWidget(dl)
	rw.addWidget(wfile)

	fedit = QTextEdit()
	flabel = QLabel('')

	conf_box = QVBoxLayout()
	conf_box.addWidget(upload)
	conf_box.addLayout(rw)
	conf_box.addWidget(flabel)
	conf_box.addWidget(fedit)

	full_comm = QHBoxLayout()
	full_comm.addLayout(comm)
	full_comm.addLayout(conf_box)

	full = QVBoxLayout()
	full.addLayout(uart)
	full.addLayout(wifi)
	full.addLayout(full_comm)

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

