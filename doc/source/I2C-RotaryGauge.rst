Rotary Gauge
------------

The Rotary Gauge is a twin-tab board that has 9 LED's that can
be used in various applications where some sort of rotary gauge
might be needed. This might include:

 - Temperature Gauge

 - Speed Gauge

 - Voltage Gauge

 - Engine RPM Gauge or anything else.

Introduction
------------

This board is based around the Phillips PCF-8574 I2C Shift-Register.

A Shift Register is a chip which takes a Serial Input, and then converts
that serial byte onto a set of pins in sequential order.

In this Gauge, the Blue LED is used to show that power is on. After that
each of the other colours (green, white, orange and red) come on depending
to the value that is received from the Microprocessor.

Each bit of the Shift-Register then maps out onto the following LED
of the Gauge:

-------------------------
Bit    Hex        | Color
-------------------------
1   |  0x01       | Green
2   |  0x02       | Green
3   |  0x04       | White
4   |  0x08       | White
3   |  0x10       | Orange
4   |  0x20       | Orange
3   |  0x40       | Red
4   |  0x80       | Red
--------------------------

Installation
------------

This board is based on the I2C Standard and can be installed simply by
inserting it into one of the I2C connectors.

By default the board is configured to exist at the I2C address of 0x3F.

On a Linux board, such as the Raspberry-Pi, you will need to install I2C
modules and drivers and that process is described >here<. 

You can find the board by connecting it up and typing the following command 
at the command shell:

> i2cdetect -y 1


Programming
-----------

This board can be programmed in any language that your microprocessor
supports. For example, C/C++, Python, Java and will work on a range
of Microprocessors.

Arduino/Wiring Example
----------------------



Raspberry-Pi/Python Example
---------------------------

#!/usr/bin/wpy

import time
import smbus
import ClixxIO_I2C

g = None
device_num = 0x3E
TIMEFORMAT = "%m/%d/%y %H:%M:%S"
INTERVAL = 2

def getTimeList():
    statFile = file("/proc/stat", "r")
    timeList = statFile.readline().split(" ")[2:6]
    statFile.close()
    for i in range(len(timeList))  :
        timeList[i] = int(timeList[i])
    return timeList

def deltaTime(interval)  :
    x = getTimeList()
    time.sleep(interval)
    y = getTimeList()
    for i in range(len(x))  :
        y[i] -= x[i]
    return y

def setup():
    global g
    g = ClixxIO_I2C.displayRotaryLeds(0x3F,1)

def loop():
    global g
    dt = deltaTime(INTERVAL)
    timeStamp = time.strftime(TIMEFORMAT)
    cpuPct = 100 - (dt[len(dt) - 1] * 100.00 / sum(dt))
    print timeStamp + "\t" + str('%.4f' %cpuPct)          

    g.display(int(cpuPct),100)

    time.sleep(0.2)







