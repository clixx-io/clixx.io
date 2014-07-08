#!/usr/bin/env python

############################################################################
#
#  Copyright (C) 2014 David Lyon
#
#  This file may be used under the terms of the GNU General Public
#  License version 2.0 as published by the Free Software Foundation
#  and appearing in the file LICENSE.GPL included in the packaging of
#  this file.  
#
#  This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
#  WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
#  This file is based on a Qt Example (c) Trolltech SA 2007
#
############################################################################

# This is only needed for Python v2 but is harmless for Python v3.
#import sip
#sip.setapi('QVariant', 2)

from PySide import QtCore, QtGui, QtWebKit
from PySide.QtCore import SIGNAL
from functools import partial

import iot_controller_rc

from clixxIO import *

class Window(QtGui.QDialog):
    def __init__(self):
        super(Window, self).__init__()

        self.createIconGroupBox()
        self.createMessageGroupBox()

        self.iconLabel.setMinimumWidth(self.durationLabel.sizeHint().width())

        self.createActions()
        self.createTrayIcon()

        self.showMessageButton.clicked.connect(self.showMessage)
        self.showIconCheckBox.toggled.connect(self.trayIcon.setVisible)
        self.iconComboBox.currentIndexChanged[int].connect(self.setIcon)
        self.trayIcon.messageClicked.connect(self.messageClicked)
        self.trayIcon.activated.connect(self.iconActivated)

        mainLayout = QtGui.QVBoxLayout()
        mainLayout.addWidget(self.iconGroupBox)
        mainLayout.addWidget(self.messageGroupBox)
        self.setLayout(mainLayout)

        self.iconComboBox.setCurrentIndex(1)
        self.trayIcon.show()

        self.setWindowTitle("Clixx.io IoT Manager")
        self.resize(400, 300)

    def setVisible(self, visible):
        self.minimizeAction.setEnabled(visible)
        self.maximizeAction.setEnabled(not self.isMaximized())
        self.restoreAction.setEnabled(self.isMaximized() or not visible)
        super(Window, self).setVisible(visible)

    def closeEvent(self, event):
        if self.trayIcon.isVisible():
        #    QtGui.QMessageBox.information(self, "Clixx.io IoT Manager",
        #            "The program will keep running in the system tray. To "
        #            "terminate the program, choose <b>Quit</b> in the "
        #            "context menu of the system tray entry.")
            self.hide()
            event.ignore()

    def setIcon(self, index):
        print index
        icon = self.iconComboBox.itemIcon(index)
        self.trayIcon.setIcon(icon)
        self.setWindowIcon(icon)

        self.trayIcon.setToolTip(self.iconComboBox.itemText(index))

    def iconActivated(self, reason):
        if reason in (QtGui.QSystemTrayIcon.Trigger, QtGui.QSystemTrayIcon.DoubleClick):
            self.iconComboBox.setCurrentIndex(
                    (self.iconComboBox.currentIndex() + 1)
                    % self.iconComboBox.count())
        elif reason == QtGui.QSystemTrayIcon.MiddleClick:
            self.showMessage()

    def showMessage(self):
        icon = QtGui.QSystemTrayIcon.MessageIcon(
                self.typeComboBox.itemData(self.typeComboBox.currentIndex()))
        self.trayIcon.showMessage(self.titleEdit.text(),
                self.bodyEdit.toPlainText(), icon,
                self.durationSpinBox.value() * 1000)

    def messageClicked(self):
        QtGui.QMessageBox.information(None, "Systray",
                "Sorry, I already gave what help I could.\nMaybe you should "
                "try asking a human?")

    def createIconGroupBox(self):
        self.iconGroupBox = QtGui.QGroupBox("Tray Icon")

        self.iconLabel = QtGui.QLabel("Icon:")

        self.iconComboBox = QtGui.QComboBox()
        self.iconComboBox.addItem(QtGui.QIcon(':/images/remote_control-1281.png'), "Remote")
        self.iconComboBox.addItem(QtGui.QIcon(':/images/radio_tower-128.png'), "Radio")
        self.iconComboBox.addItem(QtGui.QIcon(':/images/appointment_reminders-78.png'), "Appointment Reminders")  
        self.iconComboBox.addItem(QtGui.QIcon(':/images/engineering-128.png'), "Engineering")         
        self.iconComboBox.addItem(QtGui.QIcon(':/images/automotive-78.png'), "Automotive")             
        self.iconComboBox.addItem(QtGui.QIcon(':/images/robot-128.png'), "Robot")
        self.iconComboBox.addItem(QtGui.QIcon(':/images/hydroelectric-128.png'), "HydroElectric")       
        self.iconComboBox.addItem(QtGui.QIcon(':/images/sensor-128.png'), "Sensor")
        self.iconComboBox.addItem(QtGui.QIcon(':/images/bug2-128.png'), "Bug")                  
        self.iconComboBox.addItem(QtGui.QIcon(':/images/integrated_circuit-128.png'), "Integrated Circuit")  
        self.iconComboBox.addItem(QtGui.QIcon(':/images/solar_panel-78.png'), "Solar Panel")
        self.iconComboBox.addItem(QtGui.QIcon(':/images/car_battery-78.png'), "Car Battery")            
        self.iconComboBox.addItem(QtGui.QIcon(':/images/laser_beam-128.png'), "Laser Beam")          
        self.iconComboBox.addItem(QtGui.QIcon(':/images/speedometer-78.png'), "Speedometer")
        self.iconComboBox.addItem(QtGui.QIcon(':/images/circuit-1281.png'), "Circuit")              
        self.iconComboBox.addItem(QtGui.QIcon(':/images/circuit-128.png'), "Circuit2")               
        self.iconComboBox.addItem(QtGui.QIcon(':/images/led_diode-128.png'), "Led")           
        self.iconComboBox.addItem(QtGui.QIcon(':/images/speed.png'), "Speed")
        self.iconComboBox.addItem(QtGui.QIcon(':/images/nfc_checkpoint-128.png'), "NFC")      
        self.iconComboBox.addItem(QtGui.QIcon(':/images/stepper_motor-128.png'), "Stepper")
        self.iconComboBox.addItem(QtGui.QIcon(':/images/electrical_sensor-128.png'), "Sensor")     
        self.iconComboBox.addItem(QtGui.QIcon(':/images/pickup-78.png'), "Pickup")               
        self.iconComboBox.addItem(QtGui.QIcon(':/images/time.png'), "Time")
        self.iconComboBox.addItem(QtGui.QIcon(':/images/electrical_threshold-128.png'), "Threshold")  
        self.iconComboBox.addItem(QtGui.QIcon(':/images/piston-78.png'), "Piston")               
        self.iconComboBox.addItem(QtGui.QIcon(':/images/electricity-128.png'), "Electricity")           
        self.iconComboBox.addItem(QtGui.QIcon(':/images/potentiometer-1281.png'), "Potentiometer")      
        self.iconComboBox.addItem(QtGui.QIcon(':/images/walkie_talkie_radio-78.png'), "Walkie Talkie")
        self.iconComboBox.addItem(QtGui.QIcon(':/images/electronics-128.png'), "Electronics")           
        self.iconComboBox.addItem(QtGui.QIcon(':/images/pressure.png'), "Pressure")                
        self.iconComboBox.addItem(QtGui.QIcon(':/images/wind_turbine-128.png'), "Wind Turbine")
        self.iconComboBox.addItem(QtGui.QIcon(':/images/electronics-78.png'), "Electronics 2")            
        self.iconComboBox.addItem(QtGui.QIcon(':/images/engine-78.png'), "Engine")                 
        self.iconComboBox.addItem(QtGui.QIcon(':/images/relay-128.png'), "Relay")

        self.showIconCheckBox = QtGui.QCheckBox("Show icon")
        self.showIconCheckBox.setChecked(True)

        iconLayout = QtGui.QHBoxLayout()
        iconLayout.addWidget(self.iconLabel)
        iconLayout.addWidget(self.iconComboBox)
        iconLayout.addStretch()
        iconLayout.addWidget(self.showIconCheckBox)
        self.iconGroupBox.setLayout(iconLayout)

    def createMessageGroupBox(self):
        self.messageGroupBox = QtGui.QGroupBox("Balloon Message")

        typeLabel = QtGui.QLabel("Type:")

        self.typeComboBox = QtGui.QComboBox()
        self.typeComboBox.addItem("None", QtGui.QSystemTrayIcon.NoIcon)
        self.typeComboBox.addItem(self.style().standardIcon(
                QtGui.QStyle.SP_MessageBoxInformation), "Information",
                QtGui.QSystemTrayIcon.Information)
        self.typeComboBox.addItem(self.style().standardIcon(
                QtGui.QStyle.SP_MessageBoxWarning), "Warning",
                QtGui.QSystemTrayIcon.Warning)
        self.typeComboBox.addItem(self.style().standardIcon(
                QtGui.QStyle.SP_MessageBoxCritical), "Critical",
                QtGui.QSystemTrayIcon.Critical)
        self.typeComboBox.setCurrentIndex(1)

        self.durationLabel = QtGui.QLabel("Duration:")

        self.durationSpinBox = QtGui.QSpinBox()
        self.durationSpinBox.setRange(5, 60)
        self.durationSpinBox.setSuffix(" s")
        self.durationSpinBox.setValue(15)

        durationWarningLabel = QtGui.QLabel("(some systems might ignore this "
                "hint)")
        durationWarningLabel.setIndent(10)

        titleLabel = QtGui.QLabel("Title:")

        self.titleEdit = QtGui.QLineEdit("Internet-of-Things Event")

        bodyLabel = QtGui.QLabel("Body:")

        self.bodyEdit = QtGui.QTextEdit()
        self.bodyEdit.setPlainText("Something on the Intranet went Bing. Not "
                "sure what exactly it was. Check later when the program works.")

        self.showMessageButton = QtGui.QPushButton("Show Message")
        self.showMessageButton.setDefault(True)

        messageLayout = QtGui.QGridLayout()
        messageLayout.addWidget(typeLabel, 0, 0)
        messageLayout.addWidget(self.typeComboBox, 0, 1, 1, 2)
        messageLayout.addWidget(self.durationLabel, 1, 0)
        messageLayout.addWidget(self.durationSpinBox, 1, 1)
        messageLayout.addWidget(durationWarningLabel, 1, 2, 1, 3)
        messageLayout.addWidget(titleLabel, 2, 0)
        messageLayout.addWidget(self.titleEdit, 2, 1, 1, 4)
        messageLayout.addWidget(bodyLabel, 3, 0)
        messageLayout.addWidget(self.bodyEdit, 3, 1, 2, 4)
        messageLayout.addWidget(self.showMessageButton, 5, 4)
        messageLayout.setColumnStretch(3, 1)
        messageLayout.setRowStretch(4, 1)
        self.messageGroupBox.setLayout(messageLayout)
       
    def showWebpage(self, webpage):
            
        print webpage
        
        self.web = QtWebKit.QWebView()
        self.web.load(QtCore.QUrl(webpage))
        self.web.show()

        #  QObject.connect(self.view, SIGNAL('loadFinished(bool)'),
        #                  self.load_finished)
        #  url = QUrl.fromLocalFile(adjust_filename('fox.html', __file__))

    def createActions(self):
        self.minimizeAction = QtGui.QAction("Mi&nimize", self,
                triggered=self.hide)

        self.maximizeAction = QtGui.QAction("Ma&ximize", self,
                triggered=self.showMaximized)

        self.restoreAction = QtGui.QAction("&Add", self,
                triggered=self.showNormal)

        self.quitAction = QtGui.QAction("&Quit", self,
                triggered=QtGui.qApp.quit)

    def get_menu_actions(self, projectname):
        
        menu_actions = []
        
        cf = clixxIOlProjectConfigFilename(projectname)
        
        cp = SafeConfigParser()
        cp.read(cf)
   
        if cp.has_section("menu_actions"):
            menu_actions = cp.items("menu_actions")
            
        return menu_actions

    def createTrayIcon(self):
         self.trayIconMenu = QtGui.QMenu(self)
         
         projects = clixxIOListProjects()
         for d in projects:

             itmMenu = self.trayIconMenu.addMenu(d)
             
             cmds = self.get_menu_actions(d)
             for c in cmds:
             
                 actionspec = d + '/' + c[0]

                 x = actionspec
                 a = eval("lambda : performMenuAction('"+x+"')")
                 itmMenu.addAction(c[0], a)
                 # mnuAction.Checked = True
        
         self.trayIconMenu.addSeparator()
         self.trayIconMenu.addAction(self.restoreAction)
         self.trayIconMenu.addSeparator()
         self.trayIconMenu.addAction(self.quitAction)

         self.trayIcon = QtGui.QSystemTrayIcon(self)
         self.trayIcon.setContextMenu(self.trayIconMenu)

def execute_action(projectname, actionstring):

    print actionstring
    
    if actionstring.startswith("http://") or actionstring.startswith("https://"):
        
        web = QtWebKit.QWebView()
        web.load(QtCore.QUrl(actionstring))
        web.show()

    elif actionstring.startswith('mosquitto_pub'):

        pub_cmd = "mosquitto_pub" 
        topic = "mqtt|output_channel"
        topic = "clixx.io/hello"

        cmdline = "%s -h %s -t \"%s\" -m \"%s\"" % (pub_cmd,"test.mosquitto.org",topic,actionstring[len('mosquitto_pub'):])
        print "Executing %s" % cmdline

        c = spawntask(cmdline)

    else:
        os.chdir(clixxIOProjectDir(projectname))
        
        c = spawntask(actionstring)
        print c
        
def performMenuAction(identifier):

    p = identifier[:identifier.find('/')]
    c = identifier[identifier.find('/')+1:]

    menu_actions = []
        
    cf = clixxIOlProjectConfigFilename(p)
        
    cp = SafeConfigParser()
    cp.read(cf)
   
    if cp.has_section("menu_actions"):
        menu_actions = cp.items("menu_actions")
            
    for i in menu_actions:
        if (i[0].lower()==c.lower()):
            execute_action(p, i[1])

if __name__ == '__main__':

    import sys

    app = QtGui.QApplication(sys.argv)

    if not QtGui.QSystemTrayIcon.isSystemTrayAvailable():
        QtGui.QMessageBox.critical(None, "Systray",
                "I couldn't detect any system tray on this system.")
        sys.exit(1)

    QtGui.QApplication.setQuitOnLastWindowClosed(False)

    window = Window()
    # window.show()
    sys.exit(app.exec_())
