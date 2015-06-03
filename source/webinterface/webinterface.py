import platform
import csv, datetime, re

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
    config['enable_addproject'] = True

    projects = clixxIOListProjects()

    return render_template('index.html',projects = projects,config = config)

@app.route('/iot/<projectname>')
def show_project_profile(projectname):
    # show the user profile for that user
    projects = clixxIOListProjects()

    if projectname in projects:

        projectconfig_path = clixxIOlProjectConfigFilename(projectname)
        if os.path.exists(projectconfig_path):
            
            pc = open(projectconfig_path)
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
    """ Allows a user to log a sensor
    """
    projects = clixxIOListProjects()
    num_format = re.compile("^[1-9][0-9]*\.?[0-9]*")

    if projectname in projects:

        # Obtain a timestamp
        ds = datetime
        ds = ds.now()

        # Place the logfile in the project directory
        logpath = os.path.join(clixxIOProjectDir(projectname),projectname + ".csv")

        header_row = None
        if not os.path.exists(logpath):
            header_row = ['Timestamp']
            for k in request.args.keys():
                header_row.append(k)

        ofile  = open(logpath, "ab")
        writer = csv.writer(ofile, quoting=csv.QUOTE_NONNUMERIC)
        
        # Add the header row with field names
        if header_row:
            writer.writerow(header_row)  

        # Build the row and write it        
        r = [ds.isoformat(' '),]
        for k in request.args.keys():

            isnumber = re.match(num_format,request.args[k])
            if isnumber:
                r.append(float(request.args[k]))
            else:
                r.append(request.args[k])

        writer.writerow(r)  

        return 'Thank you. Those values have now been logged to %s.csv.' % projectname

    else:
        return 'Project %s is not a valid project.' % projectname

if __name__ == '__main__':
    app.run(host='0.0.0.0')
