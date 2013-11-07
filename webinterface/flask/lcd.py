#!/usr/bin/env python

# Based on the blog http://randomtutor.blogspot.com.au/2013/01/controlling-raspberrypi-from-web.html

from flask import Flask
from flask import render_template, request
# from adafruit.Adafruit_CharLCD import Adafruit_CharLCD

app = Flask(__name__)

# Initialise the LCD class
# lcd = Adafruit_CharLCD()
# lcd.begin(16,2)


@app.route("/")
def index():
    return render_template('index.html')


@app.route("/change", methods=['POST'])
def change():
    if request.method == 'POST':
        # Get the value from the submitted form
        lcdText = request.form['lcd']
        print "---Message is", lcdText
        
        # Send the message to the LCD
#        lcd.clear()
#        lcd.message(lcdText)
    else:
        lcdText = None
    return render_template('index.html', value=lcdText)


if __name__ == "__main__":
    app.debug = True
    app.run('0.0.0.0')

