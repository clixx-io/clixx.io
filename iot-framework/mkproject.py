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

import sys, os, platform, shutil, optparse
from mako.template import Template
from mako.lookup import TemplateLookup

class clixxIOEventTemplateBuilder():

	intro = """Welcome to the Clixx.io Event-Framework Program Builder.
(c) clixx.io 2014

This will build a basic Event-framework program that you
can then customise to suit your needs.

--------------------------------------------------------
"""

	project_types = { 
			   "lua-nodemcu"		: "Lua NodeMCU",
			   "c++"				: "Event-Framework",
			   "python_service"		: "Python Project",
#			   "Web_Application"	: "Web Application",
			  }

	target_platforms = {
			   "attiny13" 			: "program_setup|program_loop|program_timers|program_pinchange",
			   "attiny85" 			: "program_setup|program_loop|program_timers|program_pinchange|program_serial|program_iot",
			   "linux"    			: "program_setup|program_shutdown|program_loop|program_timers|program_pinchange|program_serial|program_iot",
			   "msp430"   			: "program_setup|program_loop|program_timers|program_pinchange|program_serial",
			   "esp8266"			: "program_setup|program_loop|program_timers|program_pinchange|program_serial"
				}

	prompts = {"program_setup" 		: "Does the program need a setup function (y,n,i) ? ",
			   "program_shutdown" 	: "Does the program need a pre-shutdown function (y,n,i) ? ",
			   "program_loop" 		: "Does the program need a polling loop (y,n,i) ? ",
			   "program_timers" 	: "Does the program need periodic timers (hardware-timer-interrupts) (y,n,i) ? ",
			   "program_pinchange" 	: "Does the program need to handle Pin Changes (hardware-pinchange-interrupts) (y,n,i) ? ",
			   "program_serial" 	: "Does the program need to handle incoming serial (hardware-serial-interrupts) (y,n,i) ? ",
			   "program_iot"		: "Does the program need to handle Internet-of-Tnings events and notifications (mqtt) (y,n,i) ? ",
			   "serial_char"		: "An Event for every serial character (y,n,i) ?", 
			   "serial_line"		: "An Event for a complete line of serial text (y,n,i) ?", 
			   "serial_open"		: "An Event for when the serial port is opened (y,n,i) ?", 
			   "serial_close"		: "An Event for when the serial port is closed (y,n,i) ?", 
			   "iot_message"		: "An Event for an IoT Subscription message (y,n,i) ?", 
			   "iot_open"			: "An Event for when the IoT channel is opened (y,n,i) ?", 
			   "iot_close"			: "An Event for when the IoT channel is closed (y,n,i) ?", 
			   "socket_server" 		: "Does the program need to handle incoming network sockets (network-socket-server) (y,n,i) ? ",
			   "socket_client" 		: "Does the program need to handle outgoing network sockets (network-socket-client) (y,n,i) ? "
			   }

	info    = {"program_loop" 		: "Normally within main(), programs have a repeating for (;;){} loop. In Wiring/Arduino, this is broken out to a loop() method. If you have code that loops, then you will need this construct.",
			   "program_timers" 	: "Programs can use timer interrupts. These are usually from hardware but on modern Operating Systems, they can be implemented in software.",
			   "program_pinchange" 	: "PinChange Interrupts occur when a GPIO pin changes. This is usually RISING/FALLING/CHANGE.",
			   "program_serial" 	: "Serial Data can captured on interrupts and fed to your program as events.",
			   "socket_server" 		: "Your program may wish to utilise a socket server to receive data from other computers",
			   "socket_client" 		: "Your program may wish to connect to other socket servers and push/pull data over a socket-stream.",
			   "mqtt_sub" 			: "Your program may wish to subscribe to Internet-of-Tnings events and notifications"
			   }

	input_dvs={"button"				: "Input Button",
			   "temp"				: "Temperature Sensor",
			   "iot_subscribe"		: "Internet-of-Things Subscribe"
			  }
			   
	output_dvs={"led"				: "Input Button",
				"rgbled"			: "RGB LED",
				"motor"				: "Electric Motor",
				"char_display"		: "Character Display",
				"graphic_display"	: "Graphics Display",
				"iot_publish"		: "Internet-of-Things Channel"
			   }

	device_cls={"button"			: "Input Button",
			   "temp"				: "Temperature Sensor",
			   "iot_subscribe"		: "Internet-of-Things Subscribe"
			  }

	iot_dir = "IoT"
    
	def __init__(self, project_name = None, deployment_platform=None):

		self.project_name = project_name
		if not self.project_name is None:
			self.projectdir = os.path.expanduser(os.path.join("~",self.iot_dir,self.project_name))

		self.templatedir = os.path.abspath('templates')
		
		if deployment_platform == None:
			self.deployment_platform = platform.system().lower()
		else:
			self.deployment_platform = deployment_platform
		
		self.selections = []
		self.project_type = "c++" 
		self.iot_commands = []
		self.iot_input_channel = ""
		self.iot_output_channel = ""
		
		return
		
	def user_prompts(self):
		"""
		Prompts the user for project settings via the command line.
		"""

		self.selections = []
		self.peripherals = []

		capabilities = self.target_platforms[self.deployment_platform]
		
		input_channel_name = ''

		print(self.intro)
		
		if self.project_name is None:
			while self.project_name is None:
				self.project_name = raw_input("What is the project name (used to make the directory and .cpp file) ? ")
		else:
			print("and the project directory will be : %s\n" % self.get_projectdir(self.project_name))
		
		if self.project_type is None:
			ptypes = "What is the project type (" + "/".join(self.project_types.keys()) + ")? "
			while not self.project_type in self.project_types.keys():
				self.project_type = raw_input(ptypes)

		if self.deployment_platform == "esp8266":
			self.project_type = "lua-nodemcu"
        
		elif self.project_type == "c++":
		
			self.deployment_platform = ''
			while not self.deployment_platform in self.target_platforms.keys():
				self.deployment_platform = raw_input("Which target platform %s ? " % str(self.target_platforms.keys()))
		
			for x in self.prompts.keys():
				if (x in capabilities) and (x.startswith('program_')):
					r = 'I'
					while not r in 'YN':
						r = raw_input(self.prompts[x]).upper()
						if r == 'Y':
							self.selections.append(x)

							# Subsection queries
							device_capabilities = []
							k = x[x.find('_')+1:]
							for y in self.prompts.keys():
								if y.startswith(k):
									r = 'I'
									while not r in 'YN':
										r = raw_input(self.prompts[y]).upper()
										if r == 'Y':
											self.selections.append(y)

						elif (r == 'I'):
							if x in self.info.keys():
								print(self.info[x])
							else:
								print("Sorry, no further information on that item.")

		if "program_iot" in self.selections:
			if self.iot_input_channel != '':
				# If there is an input_channel, collect up all the respond-to commands
				command_name = ''
				while not command_name is '':
					command_name = raw_input("Add a command (in text) of what you want this project to respond to \(eg On or Off\)? ")
					if command_name.strip() != '':
						self.iot_commands.append(command_name)
						print("%s added" % command_name)
			else:
				while input_channel_name is '':
					input_channel_name = raw_input("What IoT Channel do you want to respond to commands from ? ")
					if input_channel_name.strip() != '':
						self.iot_input_channel = input_channel_name.strip()
					else:
						break;

			# If it's not an input channel, then collect output (publish) information
			output_channel_name = ''
			while output_channel_name is '':
				output_channel_name = raw_input("What IoT Channel do you want to output to ? ")
				if input_channel_name.strip() != '':
					self.iot_output_channel = input_channel_name.strip()
					break

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

	def get_projectdir(self, projname):
		"""
		Determine a full path for a potential or existing project.
		"""
		
		return os.path.expanduser(os.path.join("~",self.iot_dir,self.project_name))

	def get_makefile_template(self, target_platform):
		"""
		Used to specifiy a Makefile template for a specific platform
		"""
	
		if target_platform.startswith('attiny'):
			return 'makefile-avr.tmpl'

		if target_platform.startswith('atmega'):
			return 'makefile-avr.tmpl'
		
		if target_platform.startswith('msp430'):
			return 'makefile-msp430.tmpl'

		return 'makefile-linux.tmpl'
		
	def create_projectdir(self):
		"""
		Create the Project Directory if it doesn't already exist
		"""
		
		self.projectdir = self.get_projectdir(self.project_name)
		
		if not os.path.exists(self.projectdir):
			print(" - Creating directory %s" % self.projectdir)
			os.makedirs(self.projectdir)
		
	def render_files(self):
		"""
		Render all the files from templates with project settings held in memory
		"""
		
		self.create_projectdir()

		self.render_makefile()

		self.render_maincppfile()

		self.render_mainhppfile()

		self.render_maincallbackfile()

		self.render_configfile()

		print("Project files successfully rendered to %s" % os.path.abspath(self.projectdir))

	def render_makefile(self):
		"""
		Render a standard gnu Makefile using a template with the settings held in memory
		"""
	
		makefile = open(os.path.join(self.projectdir,'Makefile'), 'w')

		mytemplate = Template(filename=os.path.join(self.templatedir,self.get_makefile_template(self.deployment_platform)))
		makefile.write(mytemplate.render(program_base = self.project_name, deployment_platform = self.deployment_platform))

	def render_maincppfile(self):
		"""
		Render the main .cpp file using a template with the settings held in memory
		"""
		mainfile = open(os.path.join(self.projectdir,self.project_name+'.cpp'), 'w')	
		
		mytemplate = Template(filename=os.path.join(self.templatedir,'main-cpp.tmpl'))
		mainfile.write(mytemplate.render(program_base = self.project_name, section_selections = self.selections))
		
	def render_configfile(self):
		"""
		Render the device configuration file using a template with the settings held in memory
		"""
		mainfile = open(os.path.join(self.projectdir,self.project_name+'.ini'), 'w')	
		
		mytemplate = Template(filename=os.path.join(self.templatedir,'main-config.tmpl'))
		mainfile.write(mytemplate.render(program_base = self.project_name, section_selections = self.selections, 
		                                 device_selections=self.peripherals,
		                                 iot_commands=[self.iot_commands],
		                                 iot_output_channels=[self.iot_output_channel],
		                                 iot_input_channels=[self.iot_input_channel,],
		                                 project_type=self.project_type
		                                )
		              )

	def render_mainhppfile(self):
		"""
		Render the main .hpp file using a template with the settings held in memory
		"""
		mainincludefile = open(os.path.join(self.projectdir,self.project_name+'-config.hpp'), 'w')	
		
		mytemplate = Template(filename=os.path.join(self.templatedir,'main-hpp.tmpl'))
		mainincludefile.write(mytemplate.render(program_base = self.project_name, deployment_platform = self.deployment_platform))
		
	def render_maincallbackfile(self):
		"""
		Render the main callback .cpp file using a template with the settings held in memory
		"""
	
		maincallbackfile = open(os.path.join(self.projectdir,self.project_name+'-callbacks.cpp'), 'w')	
		
		usedcalllist = {}

		usedcalllist['program_setup'] = ''
		if 'program_setup' in self.selections:
			usedcalllist['program_setup'] = 'static_cast<App*>(appC)->setup();'

		usedcalllist['program_shutdown'] = ''
		if 'program_shutdown' in self.selections:
			usedcalllist['program_shutdown'] = 'static_cast<App*>(appC)->shutdown();'

		usedcalllist['program_loop'] = ''
		if 'program_loop' in self.selections:
			usedcalllist['program_loop'] = 'static_cast<App*>(appC)->loop();'

		usedcalllist['program_pinchange'] = ''
		if 'program_pinchange' in self.selections:
			usedcalllist['program_pinchange'] = 'static_cast<App*>(appC)->pinchange();'

		usedcalllist['program_timers'] = ''
		if 'program_timers' in self.selections:
			usedcalllist['program_timers'] = 'static_cast<App*>(appC)->timer();'

		usedcalllist['serial_char'] = ''
		if 'serial_char' in self.selections:
			usedcalllist[''] = 'static_cast<App*>(appC)->serialchar();'

		usedcalllist['serial_line'] = ''
		if 'serial_line' in self.selections:
			usedcalllist['serial_line'] = 'static_cast<App*>(appC)->serialline();'

		usedcalllist['serial_open'] = ''
		if 'serial_open' in self.selections:
			usedcalllist['serial_open'] = 'static_cast<App*>(appC)->serialopen();'

		usedcalllist['serial_close'] = ''
		if 'serial_close' in self.selections:
			usedcalllist['serial_close'] = 'static_cast<App*>(appC)->serialclose();'

		usedcalllist['iot_message'] = ''
		if 'iot_message' in self.selections:
			usedcalllist['iot_message'] = 'static_cast<App*>(appC)->iotmessage();'

		usedcalllist['iot_open'] = ''
		if 'iot_open' in self.selections:
			usedcalllist['iot_open'] = 'static_cast<App*>(appC)->iotopen();'

		usedcalllist['iot_close'] = ''
		if 'iot_close' in self.selections:
			usedcalllist['iot_close'] = 'static_cast<App*>(appC)->iotclose();'

		mytemplate = Template(filename=os.path.join(self.templatedir,'main-callbacks.tmpl'))
		maincallbackfile.write(mytemplate.render(function_calls = usedcalllist,
		                       iot_commands=[self.iot_commands],
		                       iot_output_channels=[self.iot_output_channel],
		                       iot_input_channels=[self.iot_input_channel,],project_type=self.project_type)
		                      )

if __name__ == "__main__":

	usage = "usage: %prog [options] arg1 arg2"
	parser = optparse.OptionParser(usage=usage)
	parser.add_option("-t", "--target",
					  action="store", dest="platform", default=None,
					  help="Target Platform. Example Linux, atttiny85")

	(options, args) = parser.parse_args()

	project_name = None
	if len(args) > 0:
		project_name = args[0]
	
	project = clixxIOEventTemplateBuilder(project_name, deployment_platform=options.platform)
    
	project.user_prompts()

	project.render_files()


