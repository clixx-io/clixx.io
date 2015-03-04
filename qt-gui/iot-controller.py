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
from PySide.QtCore import SIGNAL, QProcess
from functools import partial
import logging, logging.config
import webbrowser, sys

from clixxIO import *
import iot_controller_rc

autostart_processlist = []
logger = logging.getLogger('iot-controller-gui')
secondwin = None

class LogFileList(QtGui.QListWidget):
    ''' A specialized QListWidget that displays the
        contents of the system log file'''
 
    def __init__(self, dirpath, parent=None):
        QtGui.QListWidget.__init__(self, parent)
        self.setDirpath(dirpath)
 
 
    def setDirpath(self, dirpath):
        ''' Set the current image directory and refresh the list. '''
        self._dirpath = dirpath
        self._populate()
 
 
    def _images(self):
        ''' Return a list of filenames of all
            supported images in self._dirpath. '''
 
        # Start with an empty list
        images = []
 
        # Find the matching files for each valid
        # extension and add them to the images list
        for extension in supported_image_extensions():
            pattern = os.path.join(self._dirpath,
                                   '*.%s' % extension)
            images.extend(glob.glob(pattern))
 
        return images
 
 
    def _populate(self):
        ''' Fill the list with images from the
            current directory in self._dirpath. '''
 
        # In case we're repopulating, clear the list
        self.clear()
 
        if not os.path.exists(clixxIOSystemLogFile()):
            return
                
        content = []
        linestoshow = 15
            
        with open(clixxIOSystemLogFile()) as f:
            content = f.readlines()
                
        if len(content) > linestoshow:
            for l in range(len(content)-linestoshow,len(content)):
                item = QtGui.QListWidgetItem(self)
                item.setText(content[l].strip())
                item.setIcon(QtGui.QIcon(':/images/circuit-128.png'))
        else:
            for l in range(len(content)-1,0):
                item = QtGui.QListWidgetItem(self)
                item.setText(content[l].strip())
                item.setIcon(QtGui.QIcon(':/images/circuit-128.png'))

        # Create a list item for each image file,
        # setting the text and icon appropriately
        #for image in self._images():
        #    item = QtGui.QListWidgetItem(self)
        #    item.setText(image)
        #    item.setIcon(QtGui.QIcon(':/images/circuit-128.png'))
 
class Window(QtGui.QDialog):
    def __init__(self):
            
        global clixxIOConfig,clixxIOConfigDir,clixxIOConfigName,clixxIOLogName, logger
        
        super(Window, self).__init__()

        self.createIconGroupBox()
        self.createMessageGroupBox()

        # self.iconLabel.setMinimumWidth(self.durationLabel.sizeHint().width())

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

        clixxIOConfig.read(clixxIOconfigPath())

        iconindex = 1
        if clixxIOConfig.has_option("GUI","iconindex"):
            iconindex  = clixxIOConfig.getint("GUI","iconindex")

        self.iconComboBox.setCurrentIndex(iconindex)
        self.trayIcon.show()

        windowTitle = "Clixx.io IoT Manager"
        if clixxIOConfig.has_option("GUI","title"):
            windowTitle = clixxIOConfig.get("GUI","title")

        self.setWindowTitle(windowTitle)
        self.resize(400, 300)

        # Create the directory for the logfile if it doesn't exist
        logfilepath = clixxIOSystemLogFile()
        if not os.path.exists(os.path.dirname(logfilepath)):
            os.makedirs(os.path.dirname(logfilepath))
            
        hdlr = logging.FileHandler(logfilepath)
        formatter = logging.Formatter('%(asctime)s %(name)s %(levelname)s %(message)s')
        hdlr.setFormatter(formatter)
        logger.addHandler(hdlr) 
        logger.setLevel(logging.DEBUG)

        # 'application' code
        logger.debug("Program Startup")

        start_autostarts()


    def setVisible(self, visible):
        self.minimizeAction.setEnabled(visible)
        self.maximizeAction.setEnabled(not self.isMaximized())
        self.restoreAction.setEnabled(self.isMaximized() or not visible)
        super(Window, self).setVisible(visible)

    def closeEvent(self, event):
        if self.trayIcon.isVisible():
            self.hide()
            event.ignore()

    def setIcon(self, index):
            
        global clixxIOConfig, logger
        
        logger.debug("Setting icon to %d" % index)
        
        icon = self.iconComboBox.itemIcon(index)
        self.trayIcon.setIcon(icon)
        self.setWindowIcon(icon)

        self.trayIcon.setToolTip(self.iconComboBox.itemText(index))

        if not clixxIOConfig.has_section("GUI"):
            clixxIOConfig.add_section("GUI")
            
        clixxIOConfig.set('GUI', 'iconindex', str(index))
        
        cfgfile = open(clixxIOconfigPath(),'w')
        clixxIOConfig.write(cfgfile)
        cfgfile.close()

    def iconActivated(self, reason):
        if reason in (QtGui.QSystemTrayIcon.Trigger, QtGui.QSystemTrayIcon.DoubleClick):
            self.iconComboBox.setCurrentIndex(
                    (self.iconComboBox.currentIndex() + 1)
                    % self.iconComboBox.count())
        elif reason == QtGui.QSystemTrayIcon.MiddleClick:
            self.showMessage()

    def showMessage(self):
                
        self.trayIcon.showMessage("Event","System is operational.")

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
        self.messageGroupBox = QtGui.QGroupBox("Log View")

#        typeLabel = QtGui.QLabel("Type:")
#        self.typeComboBox = QtGui.QComboBox()
#        self.typeComboBox.addItem("None", QtGui.QSystemTrayIcon.NoIcon)
#        self.typeComboBox.addItem(self.style().standardIcon(
#                QtGui.QStyle.SP_MessageBoxInformation), "Information",
#                QtGui.QSystemTrayIcon.Information)
#        self.typeComboBox.addItem(self.style().standardIcon(
#                QtGui.QStyle.SP_MessageBoxWarning), "Debug",
#                QtGui.QSystemTrayIcon.Warning)
#        self.typeComboBox.addItem(self.style().standardIcon(
#                QtGui.QStyle.SP_MessageBoxCritical), "None",
#                QtGui.QSystemTrayIcon.Critical)
#        self.typeComboBox.setCurrentIndex(1)
#        self.durationLabel = QtGui.QLabel("Duration:")
#        self.durationSpinBox = QtGui.QSpinBox()
#        self.durationSpinBox.setRange(5, 60)
#        self.durationSpinBox.setSuffix(" s")
#        self.durationSpinBox.setValue(15)
#        durationWarningLabel = QtGui.QLabel("(some systems might ignore this "
#                "hint)")
#        durationWarningLabel.setIndent(10)
#        titleLabel = QtGui.QLabel("Title:")
#        self.titleEdit = QtGui.QLineEdit("Internet-of-Things Event")
#        self.bodyEdit = QtGui.QTextEdit()
#        self.bodyEdit.setPlainText("Program startup.")

        self.logList = LogFileList("../webinterface/media/icons")

        self.showMessageButton = QtGui.QPushButton("System Check")
        self.showMessageButton.setDefault(True)

        bodyLabel = QtGui.QLabel("Log:")
        messageLayout = QtGui.QGridLayout()
        messageLayout.addWidget(self.logList,0, 2, 4, 4)
        #messageLayout.addWidget(typeLabel, 0, 0)
        #messageLayout.addWidget(self.typeComboBox, 0, 1, 1, 2)
        #messageLayout.addWidget(self.durationLabel, 1, 0)
        #messageLayout.addWidget(self.durationSpinBox, 1, 1)
        #messageLayout.addWidget(durationWarningLabel, 1, 2, 1, 3)
        #messageLayout.addWidget(titleLabel, 2, 0)
        #messageLayout.addWidget(self.titleEdit, 2, 1, 1, 4)
        #messageLayout.addWidget(bodyLabel, 3, 0)
        #messageLayout.addWidget(self.bodyEdit, 3, 1, 2, 4)
        messageLayout.addWidget(self.showMessageButton, 5, 4)
        messageLayout.setColumnStretch(4, 1)
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
            
        global secondwin
        
        self.minimizeAction = QtGui.QAction("Mi&nimize", self,
                triggered=self.hide)

        self.maximizeAction = QtGui.QAction("Ma&ximize", self,
                triggered=self.showMaximized)

#        self.maintainProjects = QtGui.QAction("&Maintain Projects", self,
#                triggered=self.showNormal)

#        self.SerialTerminalAction = QtGui.QAction("&Serial Terminal", self,
#                triggered=secondwin.showNormal)

#        self.SystemLogAction = QtGui.QAction("&System Log", self,
#                triggered=self.showNormal)

        self.newprojectAction = QtGui.QAction("&New Project", self,
                triggered=self.showNormal)

        self.restoreAction = QtGui.QAction("&Settings", self,
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
         #self.trayIconMenu.addAction(self.maintainProjects)
         #self.trayIconMenu.addAction(self.SerialTerminalAction)
         #self.trayIconMenu.addAction(self.SystemLogAction)
         self.trayIconMenu.addAction(self.newprojectAction)
         self.trayIconMenu.addAction(self.restoreAction)

         self.trayIconMenu.addSeparator()
         self.trayIconMenu.addAction(self.quitAction)

         self.trayIcon = QtGui.QSystemTrayIcon(self)
         self.trayIcon.setContextMenu(self.trayIconMenu)

class runProcessWindow(QtGui.QWidget):
    """"""
 
    #----------------------------------------------------------------------
    def __init__(self):
        """Constructor"""
        # super(DialogDemo, self).__init__()
        QtGui.QWidget.__init__(self)
 
        self.label = QtGui.QLabel("Python rules!")
 
        # create the buttons
        colorDialogBtn = QtGui.QPushButton("Open Color Dialog")
        fileDialogBtn =  QtGui.QPushButton("Open File Dialog")
        self.fontDialogBtn = QtGui.QPushButton("Open Font Dialog")
        inputDlgBtn = QtGui.QPushButton("Open Input Dialog")
 
        # connect the buttons to the functions (signals to slots)
        # colorDialogBtn.clicked.connect(self.openColorDialog)
        # fileDialogBtn.clicked.connect(self.openFileDialog)
        # self.fontDialogBtn.clicked.connect(self.openFontDialog)
        # self.connect(inputDlgBtn, QtCore.SIGNAL("clicked()"), self.openInputDialog)
 
        # layout widgets
        layout = QtGui.QVBoxLayout()
        layout.addWidget(self.label)
        layout.addWidget(colorDialogBtn)
        layout.addWidget(fileDialogBtn)
        layout.addWidget(self.fontDialogBtn)
        layout.addWidget(inputDlgBtn)
        self.setLayout(layout)
 
        # set the position and size of the window
        self.setGeometry(100, 100, 400, 100)
 
        self.setWindowTitle("Running Process")

def execute_action(projectname, configfile, actionstring):

    print actionstring
    
    if actionstring.startswith("http://") or actionstring.startswith("https://"):

        webbrowser.open_new_tab(actionstring)

        # This method isn't used yet
        # web = QtWebKit.QWebView()
        # web.load(QtCore.QUrl(actionstring))
        # web.show()

    elif actionstring.startswith('mosquitto_pub'):

        pub_cmd = "mosquitto_pub" 
        topic = "mqtt|output_channel"
        topic = "clixx.io/hello"

        if cp.has_section("menu_actions"):
            menu_actions = cp.items("menu_actions")
            
        cmdline = "%s -h %s -t \"%s\" -m \"%s\"" % (pub_cmd,"test.mosquitto.org",topic,actionstring[len('mosquitto_pub'):])
        print "Executing %s" % cmdline

        c = spawntask(cmdline)

    else:
            
#        processrunner = runProcessWindow()
#        processrunner.show()
            
        logger.debug("Changing to directory %s" % clixxIOProjectDir(projectname))
        os.chdir(clixxIOProjectDir(projectname))
        
        logger.info("Starting %s" % actionstring)

        c = spawntask(actionstring)
        
#        if 'error' in ''.join(c).lower():
#        if 'error' in ''.join(c).lower():
#                """
#                Show a warning message
#                """
#                flags = QtGui.QMessageBox.StandardButton.Ok
#                msg = c
#                response = QtGui.QMessageBox.warning(self, "An error occurred!", msg, flags)
#                if response == 0:
#                    logger.error(c)
                            
        
def start_autostarts():

        global autostart_processlist, logger
        
        autostarts = {}
                
        sl = clixxIOProjectAutostarts()

        for sp in sl.keys():
            
            p = QProcess()
            p.setWorkingDirectory(sl[sp]["directory"])
            
            program = sl[sp]["command"]
            arguments = sl[sp]["arguments"].split(' ')

            logger.debug("Starting %s in %s with arguments %s" % (program,sl[sp]["directory"],arguments))

            # Start the process running
            autostart_processlist.append(p)
            autostart_processlist[len(autostart_processlist)-1].start(program, arguments)

        for p in range(0,len(autostart_processlist)-1):
            logger.debug("PID=%d" % autostart_processlist[p].pid())

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
            execute_action(p, cp, i[1])

def supported_image_extensions():
    """ Get the image file extensions that can be read. 
    """
    formats = QtGui.QImageReader().supportedImageFormats()
    # Convert the QByteArrays to strings
    return [str(fmt) for fmt in formats]

if __name__ == '__main__':

    app = QtGui.QApplication(sys.argv)

    if not QtGui.QSystemTrayIcon.isSystemTrayAvailable():
        QtGui.QMessageBox.critical(None, "Systray",
                "I couldn't detect any system tray on this system.")
        sys.exit(1)

    QtGui.QApplication.setQuitOnLastWindowClosed(False)

    secondwin = runProcessWindow()
    window = Window()
    # window.show()
    sys.exit(app.exec_())
