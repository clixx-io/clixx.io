#!/usr/bin/env python

"""SocketController 

This program connect disparate systems over a tcp/ip port and provides
a server that supports many connecting producer/consumers.
"""

from optparse import OptionParser

from circuits.net.events import write
from circuits import Component, Debugger
from circuits.net.sockets import TCPServer


__version__ = "0.0.1"

USAGE = "%prog [options]"
VERSION = "%prog v" + __version__


def parse_options():
    parser = OptionParser(usage=USAGE, version=VERSION)

    parser.add_option(
        "-b", "--bind",
        action="store", type="string",
        default="0.0.0.0:8000", dest="bind",
        help="Bind to address:[port]"
    )

    parser.add_option(
        "-d", "--debug",
        action="store_true",
        default=False, dest="debug",
        help="Enable debug mode"
    )

    opts, args = parser.parse_args()

    return opts, args


class SocketController(Component):

    def init(self, args, opts):
        """Initialize our ``socketcontroller`` Component.

        This uses the convenience ``init`` method which is called after the
        component is proeprly constructed and initialized and passed the
        same args and kwargs that were passed during construction.
        """

        self.args = args
        self.opts = opts

        self.clients = {}

        if opts.debug:
            Debugger().register(self)

        if ":" in opts.bind:
            address, port = opts.bind.split(":")
            port = int(port)
        else:
            address, port = opts.bind, 8000

        bind = (address, port)

        TCPServer(bind).register(self)

    def broadcast(self, data, exclude=None):
        exclude = exclude or []
        targets = (sock for sock in self.clients.keys() if sock not in exclude)
        for target in targets:
            self.fire(write(target, data))

    def connect(self, sock, host, port):
        """Connect Event -- Triggered for new connecting clients"""

        self.clients[sock] = {
            "host": sock,
            "port": port,
            "state": {
                "nickname": None,
                "registered": False
            }
        }

        self.fire(write(sock, "Welcome to the clixx.io socket bus\n"))
        self.fire(write(sock, "Please enter a desired node name: "))

    def disconnect(self, sock):
        """Disconnect Event -- Triggered for disconnecting clients"""

        if sock not in self.clients:
            return

        nickname = self.clients[sock]["state"]["nickname"]
        self.broadcast("node=\"{0:s}\", connected=false\n".format(nickname),
                       exclude=[sock])
        del self.clients[sock]

    def read(self, sock, data):
        """Read Event -- Triggered for when client conenctions have data"""

        if not self.clients[sock]["state"]["registered"]:
            nickname = data.strip()
            self.clients[sock]["state"]["registered"] = True
            self.clients[sock]["state"]["nickname"] = nickname
            self.broadcast("node=\"{0:s}\", connected=true\n".format(nickname),
                           exclude=[sock])
        else:
            nickname = self.clients[sock]["state"]["nickname"]
            self.broadcast("node=\"{0:s}\", {1:s}\n".format(nickname, data.strip()),
                           exclude=[sock])


def main():
    opts, args = parse_options()

    # Configure and "run" the System.
    SocketController(args, opts).run()


if __name__ == "__main__":
    main()
