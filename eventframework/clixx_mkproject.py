# /usr/bin/python

intro = """Welcome to the Clixx.io Event-Framework Program Builder.

This will build a basic event-framework program that you
can then customise to suit your needs.
"""

system_capabilities = {
           "attiny13" : "program_loop|program_timers|program_pinchange",
           "attiny85" : "program_loop|program_timers|program_pinchange|program_serial"
            }

prompts = {"program_loop" : "Does the program need a polling loop ?",
           "program_timers" : "Does the program need periodic timers (hardware-timer-interrupts) ?",
           "program_pinchange" : "Does the program need to handle Pin Changes (hardware-pinchange-interrupts) ?",
           "program_serial" : "Does the program need to handle incoming serial (hardware-serial-interrupts) ?",
           "program_network_socket_server" : "Does the program need to handle incoming network sockets (network-socket-server) ?",
           "program_network_socket_client" : "Does the program need to handle outgoing network sockets (network-socket-client) ?"
           }
           
def user_prompts(target_platform):

    selections = []
    capabilities = system_capabilities[target_platform]
    
    print intro

    print "Target platform is %s\n" % target_platform
    
    for x in prompts.keys():
        if x in capabilities:
            if raw_input(prompts[x]).upper() == 'Y':
                selections.append(x)
            
    print selections

if __name__ == "__main__":

    target_platform = 'attiny85'
    
    user_prompts(target_platform)
