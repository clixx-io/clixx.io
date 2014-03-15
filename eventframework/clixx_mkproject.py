# /usr/bin/python

"""
 * Copyright (c) 2014 clixx.io
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of the Clixx.io nor the names of its contributors 
 *    may be used to endorse or promote products derived from this software 
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL CLIXX.IO BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES LOSS OF USE, DATA, 
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
"""

import sys, os, platform
from mako.template import Template
from mako.lookup import TemplateLookup

class clixxIOEventTemplateBuilder():
	
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
		"""
		Prompts the user for project settings via the command line.
		"""

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

		print("\nPeripheral Entry (What is connected to the processor).\n---------------------------------------------\n"
		      "\nEnter <blank> to finish\n")
		peripheral_name = '-'
		while not peripheral_name is '':
			peripheral_name = raw_input("Enter the name or type of peripheral that you wish to connect ? ")
            
			if peripheral_name.strip() != '':
				self.peripherals.append(peripheral_name)
				print("%s added" % peripheral_name)
				
		print("Target platform is %s\n" % self.deployment_platform)
		print("Selections are :", self.selections)
		print("Peripherals are:", self.peripherals)
		
	def create_projectdir(self):
		"""
		Create the Project Directory if it doesn't already exist
		"""
		
		if not os.path.exists(self.project_name):
			os.makedirs(self.project_name)
			
	def render_files(self):
		"""
		Render all the files from templates with project settings held in memory
		"""
		
		self.create_projectdir()
			
		self.render_makefile()
			
		self.render_maincppfile()
			
		self.render_mainhppfile()
			
		self.render_maincallbackfile()
		
		print("Project files successfully rendered to %s" % os.path.abspath(self.project_name))

	def render_makefile(self):
		"""
		Render a standard gnu Makefile using a template with the settings held in memory
		"""
	
		makefile = open(os.path.join(self.project_name,'Makefile'), 'w')	
		
		mytemplate = Template(filename='templates/makefile-avr.tmpl')
		makefile.write(mytemplate.render(program_base = self.project_name, deployment_platform = self.deployment_platform))

	def render_maincppfile(self):
		"""
		Render the main .cpp file using a template with the settings held in memory
		"""
		
		pass
		
	def render_mainhppfile(self):
		"""
		Render the main .hpp file using a template with the settings held in memory
		"""
		
		pass
		
	def render_maincallbackfile(self):
		"""
		Render the main callback .cpp file using a template with the settings held in memory
		"""
	
		pass
		
		
if __name__ == "__main__":

	project = clixxIOEventTemplateBuilder('myproject','attiny85')
    
	project.user_prompts()
	
	project.render_files()
    

