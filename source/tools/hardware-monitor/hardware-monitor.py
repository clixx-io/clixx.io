#!/usr/bin/env python
"""Simple Timers

A trivial simple example of using circuits and timers.

"""

from circuits import Event, Component, Timer
import platform, serial, glob

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

class App(Component):

    timer_interval = 5
    serial_ports = []
    network_ips = []

    def timer(self):
        """ Timer Event handler
        Fired once in ${timer_interval} seconds.
        """

        current_ports = self.read_serial_ports()
        for p in current_ports:
			if not p in self.serial_ports:
				print "New serial port %s found" % p
				self.serial_ports.append(p)

        for p in self.serial_ports:
			if not p in current_ports:
				print "Serial port %s disconnected" % p
				i = self.serial_ports.index(p)
				del self.serial_ports[i]				

    def read_serial_ports(self):
		return list_serial_ports()
		
    def read_network_ips(self):
		return
		
    def started(self, component):
		"""started Event handler

		"""

		self.serial_ports = self.read_serial_ports()

		# Timer(seconds, event, persist=False)
		Timer(self.timer_interval, Event.create("timer"), persist=True).register(self)

App().run()

