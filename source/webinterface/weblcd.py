#!/usr/bin/env python

# Based on the blog http://randomtutor.blogspot.com.au/2013/01/controlling-raspberrypi-from-web.html

from flask import Flask
from flask import render_template, request
from clixxIO_CharLCD import i2cLCD
import wiringpi2

app = Flask(__name__)

# Initialise the LCD class
wiringpi2.wiringPiSetupPhys()
dev = wiringpi2.wiringPiI2CSetup(0x20)
if dev < 0:
  print "ERROR: Could not connect to device!"
  exit(1)

lcd = i2cLCD(dev)
lcd.setup()
lcd.clear()
lcd.write('clixx.io WebLCD')

@app.route("/")
def index():
    return render_template('weblcd.html')


@app.route("/change", methods=['POST'])
def change():
    if request.method == 'POST':
        # Get the value from the submitted form
        lcdText = request.form['lcd']
        print "---Message is", lcdText

        # Send the message to the LCD
        lcd.gotoXY(0,1)
        lcd.write(lcdText)
    else:
        lcdText = None
    return render_template('index.html', value=lcdText)


if __name__ == "__main__":
    app.debug = True
    app.run('0.0.0.0',80)

