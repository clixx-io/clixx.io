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
debug = True
        
def main(args):
	
	# _serial will handle our serial ports
	_serial = serial.Serial()

	#_serial_events handles events for the _serial object defined above
	_serial_events = SerialEvents(_serial)
	
	#_port will store the current serial port we are trying to connect to.
	_connected_serial_port = ""
	
	#_window will store our main UI
	_window =  None
	
	_text_console = None
	
	#TODO
	def readChipID():
		_serial.write("print(node.chipid())\n")
	#TODO
	def showip():
		_serial.write('print(wifi.sta.getip())\n')

	#TODO
	def list_accesspoints():
		""" This function will print out the Access-Point list """
		_access_points = ['wifi.setmode(wifi.STATION)\n',
                  "function listap(t)\n",
                  "  for k,v in pairs(t) do\n",
                  "    print(k..\" : \"..v)\n",
                  "  end\n",
                  "end\n",
                  "print('***Wifi Networks:***')\n",
                  "wifi.sta.getap(listap)\n"]
		for _access_point in _access_points:
			_serial.write(_access_point)
			sleep(.5)
	#TODO
	def setwifi():
		_serial.write('wifi.setmode(wifi.STATION)\n')
		_serial.write('wifi.sta.config("%s", "%s")\n' % (str(ssidtext.text()), str(pwdtext.text())))
		_serial.write('wifi.sta.connect()\n')
		#print 'status\n', se.readAll().decode("utf-8")

	#TODO
	def sendStr():
		t = str(sendtext.toPlainText()+'\n')
		print "Text=",t
		if len(t)>0:
			_serial.write(t)
	def write():
		#print 'NEW DATA?',se.readAll().decode("utf-8")
		textedit.setText(textedit.toPlainText() + _serial_events.readAll().decode("utf-8"))
		return

	#TODO
	def restart():
		textedit.setText("")
		_serial.write('node.restart()\n')
	
	def append_to_console(text):
		global _text_console
		# msgBox = QMessageBox()
		# msgBox.setText(text)
		# msgBox.exec_()
		_text_console.append(text)

	def _init_gui(_window):
		global _text_console
		
		_window.setWindowTitle('Terminal for ESP8266')
		
		#Let's center the main window
		
		window_min_width = 500
		window_min_height = 500
		_window.setMinimumSize(window_min_width, window_min_height)
		desktop = QDesktopWidget()

		screen_width = desktop.screen().width()
		screen_height = desktop.screen().height()

		_window.setGeometry((screen_width/2)-(window_min_width/2),(screen_height/2)-(window_min_height/2),window_min_width,window_min_height)

		
		#Let's initialize the status bar.
		_window.statusBar().showMessage("Not Connected");
		
		#Let's initialize our central widget. All main components will be lined up vertically.
		_central_widget = QWidget()
		_layout_central_widget = QVBoxLayout()
		_central_widget.setLayout(_layout_central_widget)
		_window.setCentralWidget(_central_widget)
		
		
		###
		###Let's add the connection groupbox
		###
		
		_groupbox_connection = QGroupBox("Connection")
		_layout_central_widget.addWidget(_groupbox_connection)
		layout_groupbox_connection = QHBoxLayout()
		_groupbox_connection.setLayout(layout_groupbox_connection)
		
		#The connection groupbox has a Serial Port selection QComboBox
		_combobox_serial_ports = QComboBox()
		_combobox_serial_ports.addItem("Select Serial Port")
		#we need to fill the serial port combobox with predefined system serial ports.
		#The function below tries to list serial ports on most common platforms
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

		#Now that we have a function that lists all system serial ports, let's add it to our combobox.
		_serial_ports = list_serial_ports()
		for _serial_port in reversed(_serial_ports):
			_combobox_serial_ports.addItem(_serial_port)
		
		layout_groupbox_connection.addWidget(_combobox_serial_ports)
		
		#The connection groupbox has a connect QPushButton
		_button_connect = QPushButton("Connect")
		#The connection buttion is initially disabled.
		_button_connect.setEnabled(False)
		layout_groupbox_connection.addWidget(_button_connect)
		
		def _button_connect_clicked():
			#first we retrieve the current selected serial port
			_serial_port = _combobox_serial_ports.currentText()
			append_to_console("Connection Attempt To: [" + _serial_port+ "]")
			if not _serial.isOpen():
				try:
					_serial.port = _serial_port
					_serial.open()
					_window.statusBar().showMessage("Connected To: " + _serial_port)
					append_to_console("Connected To: " + _serial_port)
					_current_serial_port = _serial_port
				except (OSError, IOError) as e:
					_error = "I/O error({0}): {1}".format(e.errno, e.strerror)
					append_to_console(_error);

		
		_button_connect.clicked.connect(_button_connect_clicked)
		
		#we activate the connection button only when a valid serial port is selected.

		def _combobox_serial_ports_currentIndexChanged(currentIndex):
			try:
				_button_connect.setEnabled(True)
				if currentIndex == 0:
					_button_connect.setEnabled(False)
			except (OSError, IOError) as e:
					print "I/O error({0}): {1}".format(e.errno, e.strerror)
		
			
		_combobox_serial_ports.currentIndexChanged.connect(_combobox_serial_ports_currentIndexChanged)
		
		
		###
		###Let's add the wifi groupbox
		###
		
		_groupbox_wifi = QGroupBox("Wi-Fi")
		_layout_central_widget.addWidget(_groupbox_wifi)
		layout_groupbox_wifi = QHBoxLayout()
		_groupbox_wifi.setLayout(layout_groupbox_wifi)

		
		#The wifi groupbox has an SSID QComboBox
		_combobox_ssid = QComboBox()
		_combobox_ssid.addItem("Select SSID")
		_combobox_ssid.addItem("SSID1")
		_combobox_ssid.addItem("SSID2")
		layout_groupbox_wifi.addWidget(_combobox_ssid)
		
		#The wifi groupbox has a password field
		_text_wifi_password = QLineEdit()
		_text_wifi_password.setEchoMode(QLineEdit.Password)
		_text_wifi_password.setPlaceholderText("password")
		layout_groupbox_wifi.addWidget(_text_wifi_password)
		
		#The wifi groupbox has a setup QPushButton
		_button_wifi_setup = QPushButton("Setup Wi-Fi")
		layout_groupbox_wifi.addWidget(_button_wifi_setup)
		
		# #The wifi groupbox has a show ips QPushButton
		# _button_show_ips = QPushButton("Show IPs")
		# layout_groupbox_wifi.addWidget(_button_show_ips)
		
		# #The wifi groupbox has a show ips QPushButton
		# _button_list_ap = QPushButton("List AP")
		# layout_groupbox_wifi.addWidget(_button_list_ap)
		
		
		###
		###Let's add the console groupbox
		###
		
		_groupbox_console = QGroupBox("Device Console")
		_layout_central_widget.addWidget(_groupbox_console)
		layout_groupbox_console = QHBoxLayout()
		_groupbox_console.setLayout(layout_groupbox_console)
		
		#the console groupbox has a console text field
		_text_console = QTextEdit()
		_text_console.append("Waiting for connection...")
		_text_console.setReadOnly(True)
		_text_console.setStyleSheet("QTextEdit {color:green;  background-color: black;  }");
		layout_groupbox_console.addWidget(_text_console)
		
		_layout_central_widget.addStretch()
		
		
		###
		###Let's add the command groupbox
		###
		
		_groupbox_command = QGroupBox("Command Center")
		_layout_central_widget.addWidget(_groupbox_command)
		layout_groupbox_command = QGridLayout()
		_groupbox_command.setLayout(layout_groupbox_command)
		
		#The command groupbox has a quick actions section
		layout_groupbox_command.addWidget(QLabel("Select a Quick Action below:"), 0, 0)
		_list_actions = QListWidget()
		
		def _list_actions_currentTextChanged(currentText):
			append_to_console(currentText)
			
		_list_actions.currentTextChanged.connect(_list_actions_currentTextChanged)
		
		_actions = ["restart", "show ip", "list ap", "flash firmware", "upload project"]
		
		for _action in _actions:
			_list_actions.addItem(_action)
			

		
		layout_groupbox_command.addWidget(_list_actions, 1, 0)
		
		#the command groupbox has a command text field
		layout_groupbox_command.addWidget(QLabel("Or type your commands below:"), 0,1)
		_text_command = QTextEdit()
		layout_groupbox_command.addWidget(_text_command, 1,1)
		
		_layout_central_widget.addStretch()
	
		
	def _serial_events_readyRead():
		msg = _serial_events.readAll().decode("utf-8")
		print "something new in serial port", msg
		append_to_console(msg)
		
		
	def _init_serial_events():
		_serial_events.readyRead.connect(_serial_events_readyRead)
		_serial_events.start()
		
#All functions declared, let's start the main workflow.


#let's initialize our _serial_events
	_init_serial_events()
#let's initialize our application
	_app = QtGui.QApplication(args)
#let's initialize our window
	_window = QMainWindow()
	_init_gui(_window)


	_window.show()
	def aboutToQuit():
		print "about to quit"
		_serial_events.stop()
		_serial.close()
		os._exit(0)
	
	_app.aboutToQuit.connect(aboutToQuit)
	
	r = _app.exec_()
	
	#return r

if __name__ == "__main__":
	main(sys.argv)