import platform

from flask import Flask
from flask import render_template, request, redirect

from clixxIO import *

app = Flask(__name__)

@app.route('/')
def index():
    """ Generate a home page with a list of projects
    """
    config = {}
    config['name'] = "Clixx.io IoT running on %s" % platform.node()
    projects = clixxIOListProjects()
    
    return render_template('index.html',projects = projects,config = config)

@app.route('/iot/<projectname>')
def show_project_profile(projectname):
    # show the user profile for that user
    projects = clixxIOListProjects()
    
    if projectname in projects:
        
        pc = open(clixxIOlProjectConfigFilename(projectname))
        lines = pc.read()
        pc.close()
        
        config = {}
        config['name'] = projectname
        config['enable_onoff'] = False
        config['enable_table'] = True
        
        commands = ["Stop", "Start","Restart"]
       
        return render_template('project.html',commands = commands,config = config)
        
    else:
        return 'Project %s is not a valid project.' % projectname

@app.route('/new_project', methods=['POST', 'GET'])
def new_project():
    """ Set up a new IoT project
    """
   
    config = {}
    config['name'] = 'New Project'
    config['enable_onoff'] = False
    config['enable_table'] = True

    commands = ["Stop", "Start","Restart"]

    if request.method == 'POST':
		
        projectname = request.form['projectname']
        print("The Project name is '" + projectname + "'")
           
    return render_template('new-project.html',commands = commands,config = config)

@app.route('/logsensor/<projectname>')
def log_sensor(projectname):
    # Allows a user to log a sensor
    projects = clixxIOListProjects()
    
    if projectname in projects:
        
        pc = open(clixxIOlProjectConfigFilename(projectname))
        lines = pc.read()
        pc.close()
        
        config = {}
        config['name'] = projectname
        config['enable_onoff'] = False
        config['enable_table'] = True
        
        commands = ["Stop", "Start","Restart"]
       
        return 'Values logged for %s.' % projectname
        
    else:
        return 'Project %s is not a valid project.' % projectname

if __name__ == '__main__':
    app.run(host='0.0.0.0')
