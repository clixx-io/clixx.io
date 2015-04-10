#!/usr/bin/env python
"""Serial Port Monitor

A Timer based utility to check for serial ports added or removed from
the system.

"""

from circuits import Event, Component, Timer
import platform, glob, subprocess
from socket import socket, SOCK_DGRAM, AF_INET 
    
def ping_address(ip_address):
    
    if platform.system() == "Windows":
        packet_count_option = "-n"
    else:
        packet_count_option = "-c"
    
    output = None
    try:
        print("Checking host %s" % ip_address)
        output=subprocess.check_output(["ping",packet_count_option,"1",ip_address])
    except:
        pass
        
    return output

def get_mac_address(ip_address):
    
    output = None
    if platform.system() == "Windows":
        try:
            print("Getting MAC address %s" % ip_address)
            output=subprocess.check_output(["nbtstat","-a",ip_address])
        except:
            pass
    
    return output

def host_alive(ip_address):
    r = ping_address(ip_address)
    if r is None:
        return False
    
    if platform.system() == "Windows":
        if r.find('Destination host unreachable') != -1:
            print "Host not detected"
            return False
        else:
            print "Host Alive"
            return True

    if (r.find('0% packet loss') != -1) or (r.find('0% loss') != -1):
        return True
    else:
        return False
    
def get_net_if():
    if_name = None
    if platform.system() == "Windows":
        return "eth0"
    else:
        netif = open("/proc/net/dev")
        contents = netif.read()
        if_pos = contents.find("eth")
        if if_pos != -1:
            # Search up to the :, it's the last character in the device name
            if_len = contents.find(':',if_pos)
            if if_len != -1:
                if_name = contents[if_pos:if_len]
    return if_name
        
def get_ip_address(ifname):
    if platform.system() == "Windows":
        return socket.gethostbyname(socket.getfqdn())
    else:
        s = socket(AF_INET, SOCK_DGRAM)
        s.connect(('google.com',0))
        return s.getsockname()[0]

def get_ip_net_base(ip_address):
    last_dot_pos = ip_address.rfind('.')
    if last_dot_pos != -1:
        return ip_address[0:last_dot_pos+1]
    else:
        return None
    
def build_ip_subnet(base_ip_address,expected_hosts = 16):

    subnet = []
    subnet_ipbase = get_ip_net_base(base_ip_address)
    
    for i in range(1,expected_hosts):
        
        subnet.append(subnet_ipbase + str(i))
        
    return subnet

class App(Component):

    timer_interval = 2
    network_watch_ips = []
    alive_hosts = []
    net_iface = None
    net_ip = None

    def timer(self):
        """ Timer Event handler
        Fired once in ${timer_interval} seconds.
        """

        current_ips = self.read_network_ips()
        for p in current_ips:
            if not p in self.alive_hosts:
                print "New IP addresses %s found" % p
                self.alive_hosts.append(p)

        for p in self.alive_hosts:
            if not host_alive(p):
                print "IP addresses %s disconnected" % p
                i = self.alive_hosts.index(p)
                del self.alive_hosts[i]

    def read_network_ips(self):
        
        alive_list = []
        
        for ip in self.network_watch_ips:
            if host_alive(ip):
                alive_list.append(ip)
        return alive_list

    def started(self, component):
        """started Event handler

        """
        self.net_iface = get_net_if()
        self.net_ip = get_ip_address(self.net_iface)
        print("Starting on Network Interface %s with IP=%s" % (self.net_iface,self.net_ip))

        print get_ip_net_base(self.net_ip)
        
        self.network_watch_ips = build_ip_subnet(self.net_ip)
        
        print(self.network_watch_ips)

        # Timer(seconds, event, persist=False)
        Timer(self.timer_interval, Event.create("timer"), persist=True).register(self)

App().run()

