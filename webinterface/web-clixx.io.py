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

@app.route('/iot/<projectname>')
def show_project_profile(projectname):
    # show the user profile for that user
    projects = clixxIOListProjects()
    if projectname in projects:
        pc = open(clixxIOlProjectConfigFilename(projectname))
        lines = pc.read()
        pc.close()
        return "<h1>%s</h1>" % projectname + "<p>" + lines + "</p>"
        
    else:
        return 'Project %s is not a valid project.' % projectname

if __name__ == '__main__':
    app.run(host='0.0.0.0')
