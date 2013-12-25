Coin Cell Battery Board
=======================

.. image:: images/LED-Board.png

The Coin Cell Battery Board provides a power source for the
clixx.io range of processor boards.

Normally, most Coin Cell batteries provide a voltage of 3.0v
at very low current. It's usually enough to run one of the
processor boards for a considerable amount of time, provided
that the low power usage is enabled in the software.

Connection
----------

The battery board simply plugs into the processor board
in at either the top or bottom.

Special Considerations
----------------------

The output-pin is connected through a current-blocking resistor
to enable the battery voltage to be read with an ADC port if
you need to monitor the voltage to determine the battery condition.
