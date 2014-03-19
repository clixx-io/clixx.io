#!/usr/bin/env python

"""Simple Timer

A trivial simple example of using circuits and timers.
"""

from circuits import Event, Component, Timer

class App(Component):

    def timerevent(self):
        """hello Event handler

        Fired once every second.
        """

        print("Timer Event")

    def started(self, component):
        """started Event handler

        Setup a simple timer to fire every second.
        """

        # Timer(seconds, event, persist=False)
        Timer(1, Event.create("timerevent"),persist=True).register(self)

App().run()
