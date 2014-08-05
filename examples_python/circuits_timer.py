!/usr/bin/env python

"""Simple Timers

A trivial simple example of using circuits and timers.
"""

from circuits import Event, Component, Timer

class App(Component):

    def hello(self):
        """hello Event handler

        Fired once in 5 seconds.
        """

        print("Hello World")

    def started(self, component):
        """started Event handler

        Setup 3 timers at 5, 1 and 3 seconds.
        The 2nd two timers are persitent meaning that
        they are fired repeatedly every 1 and 3 seconds
        respectively.
        """

        # Timer(seconds, event, persist=False)
        Timer(.2, Event.create("hello")).register(self)

App().run()
