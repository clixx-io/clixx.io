# /usr/bin/python

from mako.template import Template
from mako.lookup import TemplateLookup

class TemplateBuilder():
	
	intro = """Welcome to the Clixx.io Event-Framework Program Builder.
(c) clixx.io 2014

This will build a basic event-framework program that you
can then customise to suit your needs.

--------------------------------------------------------
"""

	system_capabilities = {
			   "attiny13" : "program_loop|program_timers|program_pinchange",
			   "attiny85" : "program_loop|program_timers|program_pinchange|program_serial"
				}

	prompts = {"program_loop" : "Does the program need a polling loop (y,n,i) ? ",
			   "program_timers" : "Does the program need periodic timers (hardware-timer-interrupts) (y,n,i) ? ",
			   "program_pinchange" : "Does the program need to handle Pin Changes (hardware-pinchange-interrupts) (y,n,i) ? ",
			   "program_serial" : "Does the program need to handle incoming serial (hardware-serial-interrupts) (y,n,i) ? ",
			   "program_network_socket_server" : "Does the program need to handle incoming network sockets (network-socket-server) (y,n,i) ? ",
			   "program_network_socket_client" : "Does the program need to handle outgoing network sockets (network-socket-client) (y,n,i) ? "
			   }

	info    = {"program_loop" : "Normally within main(), programs have a repeating for (;;){} loop. In Wiring/Arduino, this is broken out to a loop() method. If you have code that loops, then you will need this construct.",
			   "program_timers" : "Programs can use timer interrupts. These are usually from hardware but on modern Operating Systems, they can be implemented in software.",
			   "program_pinchange" : "PinChange Interrupts occur when a GPIO pin changes. This is usually RISING/FALLING/CHANGE.",
			   "program_serial" : "Serial Data can captured on interrupts and fed to your program as events.",
			   "program_network_socket_server" : "Your program may wish to utilise a socket server to receive data from other computers",
			   "program_network_socket_client" : "Your program may wish to connect to other socket servers and push/pull data over a socket-stream."
			   }

	def __init__(self, project_name,deployment_platform='local'):

		self.project_name = project_name
		self.deployment_platform = deployment_platform
		self.selections = []
		
		return
		
	def user_prompts(self):

		self.selections = []
		self.peripherals = []

		capabilities = self.system_capabilities[self.deployment_platform]
		
		print self.intro
		
		while self.project_name is None:
			self.project_name = raw_input("What is the project filename (used to make the .cpp file) ? ")
		
		for x in self.prompts.keys():
			if x in capabilities:
				r = 'I'
				while not r in 'YN':
					r = raw_input(self.prompts[x]).upper()
					if r == 'Y':
						self.selections.append(x)
					elif (r == 'I'):
						if x in self.info.keys():
							print(self.info[x])
						else:
							print("Sorry, no further information on that item.")

		print("\nPeripheral Entry (What is connected to the processor).\n---------------------------------------------\n")
		peripheral_name = '-'
		while not peripheral_name is '':
			peripheral_name = raw_input("Enter the name or type of peripheral that you wish to connect ? ")
            
			if peripheral_name.strip() != '':
				self.peripherals.append(peripheral_name)
				print("%s added" % peripheral_name)
				
		print("Target platform is %s\n" % self.deployment_platform)
		print("Selections are :", self.selections)
		print("Peripherals are:", self.peripherals)
		
	def render_files(self):
		
		mylookup = TemplateLookup(directories=['/docs'])
		mytemplate = Template("""<%include file="header.txt"/> hello world!""", lookup=mylookup)

	def render_makefile(self):
		
		pass

	def render_maincppfile(self):
		
		pass
		
	def render_mainhppfile(self):
		
		pass
		
	def render_maincallbackfile(self):
		
		pass
		
		
if __name__ == "__main__":

    project = TemplateBuilder(None,'attiny85')
    
    project.user_prompts()
