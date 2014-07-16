from flask import Flask
from flask import render_template, request

from clixxIO import *

app = Flask(__name__)

@app.route('/')
def index():
    """ Generate a home page with a list of projects
    """
    config = {}
    config['name'] = "Lesters Insurance Machine"
    projects = clixxIOListProjects()
    
    return render_template('index.html',projects = projects,config = config)
    # return "<h1>Welcome to clixx.io</h1>" + plist

@app.route('/iot/<projectname>')
def show_project_profile(projectname):
    # show the user profile for that user
    projects = clixxIOListProjects()
    
    if projectname in projects:
        
        pc = open(clixxIOlProjectConfigFilename(projectname))
        lines = pc.read()
        pc.close()
        # pc.close()
        
        config = {}
        config['name'] = projectname
        config['enable_onoff'] = False
        config['enable_table'] = True
        
        commands = ["Stop", "Start","Restart"]
       
        return render_template('project.html',commands = commands,config = config)
        
        # return "<h1>%s</h1>" % projectname + "<p>" + lines + "</p>"
        
    else:
        return 'Project %s is not a valid project.' % projectname

if __name__ == '__main__':
    app.run(host='0.0.0.0')
