from flask import Flask

from clixxIO import *

app = Flask(__name__)

@app.route('/')
def hello_world():
""" Generate a home page with a list of projects
    """
    plist = ""
    projects = clixxIOListProjects()
    for p in projects:
        plist = plist + "<p>" + p + "</p>"
    
    return "<h1>Welcome to clixx.io</h1>" + plist

if __name__ == '__main__':
    app.run(host='0.0.0.0')
