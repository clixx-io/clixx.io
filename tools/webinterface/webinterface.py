#!/usr/bin/python
import platform, StringIO
import csv, datetime, re
from time import strftime

from flask import Flask
from flask import render_template, request, redirect, send_from_directory

from clixxIO import *

app = Flask(__name__)

editorlines = """
--[[
example useless code to show lua syntax highlighting
this is multiline comment
]]

function blahblahblah(x)

  local table = {
    "asd" = 123,
    "x" = 0.34,  
  }
  if x ~= 3 then
    print( x )
  elseif x == "string"
    my_custom_function( 0x34 )
  else
    unknown_function( "some string" )
  end

  --single line comment
  
end

function blablabla3()

  for k,v in ipairs( table ) do
    --abcde..
    y=[=[
  x=[[
      x is a multi line string
   ]]
  but its definition is iside a highest level string!
  ]=]
    print(" \"\" ")

    s = math.sin( x )
  end

end
"""

@app.route('/')
def index():
    """ Generate a home page with a list of projects
    """

    w = clixxIOProject('webinterface')
        
    config = {}
    config['name'] = w.getConfig('display','system_name',"IoT running on %s" % platform.node())
    config['enable_addproject'] = False

    projects = {}

    for pn in clixxIOListProjects():
        
        p = clixxIOProject(pn)
        
        if p.getConfig('Web_Interface','show_icon','true').lower() == 'false':
            continue
        
        # Add a dictionary for every project
        pi = {}
        pi['name'] = pn
        pi['tiledescription'] = p.getConfig('Web_Interface','tiledescription')
        pi['tileimage']       = p.getConfig('Web_Interface','tileimage')
        pi['tilecolor']       = p.getConfig('Web_Interface','tilecolor')
        pi['http_redirect']   = p.getConfig('Web_Interface','http_redirect')
        pi['iconaction']      = p.getConfig('Web_Interface','iconaction')
        pi['statuslabel']     = p.getConfig('Web_Interface','statuslabel')
        pi['statusvalue']     = p.getConfig('Status','last_value')
        
        projects[pn] = pi
        

    return render_template('index.html',projects = projects,config = config)

@app.route('/iot/<projectname>')
def show_project_profile(projectname):
    
    max_lines = 40
    
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
        config['csv_data'] = False
        config['log_data'] = False
        config['ini_data'] = False

        commands = [] # "Stop", "Start","Restart"]

        cfgfile_lines = []
        logfile_lines = []

        projectdir = clixxIOProjectDir(projectname)
        csvname = os.path.join(projectdir,projectname + '.csv')
        cfgname = os.path.join(projectdir,projectname + '.ini')
        logname = os.path.join(projectdir,projectname + '.log')
        
        if os.path.exists(csvname):

            # We don't actually send the data here, just indicate
            # that it's available.
            config['csv_data'] = True
        
        if os.path.exists(logname):
            # Read in the logfile
            lf = open(logname)
            logfile_lines = lf.readlines()
            lf.close()
            
            # Trim the file
            if len(logfile_lines) > max_lines:
                for i in range(0,len(logfile_lines)-max_lines):
                    del logfile_lines[0]
                    
            # Reverse the list so newest entries are at the top
            logfile_lines.reverse()
                
            config['log_data'] = True

        if os.path.exists(cfgname):
            # Read in the logfile
            lf = open(cfgname)
            cfgfile_temp = lf.readlines()
            lf.close()
            
            for l in cfgfile_temp:
                cfgfile_lines.append(l.strip())
            
            # Trim the file
            if len(cfgfile_lines) > max_lines:
                for i in range(0,len(cfgfile_lines)-max_lines):
                    del cfgfile_lines[0]
                    
            # cfgfile_lines = ["Line 1","","Line 3"]
            config['ini_data'] = True

        return render_template('project.html',commands = commands,config = config, logfile = logfile_lines, cfgfile = cfgfile_lines)

    else:
        return 'Project %s is not a valid project.' % projectname

@app.route('/editor/<projectname>')
def get_project_editor(projectname):


    
    # show the user profile for that user
    projects = clixxIOListProjects()
    if projectname in projects:
		
        config = {}
        config['name'] = projectname
        config['enable_onoff'] = False
        config['enable_table'] = True
        config['csv_data'] = False
        config['log_data'] = False
        config['ini_data'] = False
        		
        return render_template('editor.html',config = config, editorfile = editorlines.split('\n'))

    else:
        return 'Project %s is not a valid project.' % projectname

@app.route('/csvdata/<projectname>')
def get_project_csvdata(projectname):
    
    # show the user profile for that user
    projects = clixxIOListProjects()
    if projectname in projects:

        return send_from_directory(clixxIOProjectDir(projectname),projectname+".csv", as_attachment=False)

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

        # Place the logfile in the project directory
        logpath = os.path.join(clixxIOProjectDir(projectname),projectname + ".csv")

        header_row = None
        if not os.path.exists(logpath):
            header_row = ['Timestamp']
            for k in request.args.keys():
                header_row.append(k)

        ofile  = open(logpath, "ab")
        writer = csv.writer(ofile, quoting=csv.QUOTE_MINIMAL)
        
        # Add the header row with field names
        if header_row:
            writer.writerow(header_row)  

        # Obtain a timestamp. It needs to be formatted this way to support
        # dygraphs. 
        ds = strftime("%Y-%m-%d %H:%M:%S")

        # Build the row and write it        
        r = [ds,]
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

    if os.name in ['posix'] and os.geteuid() == 0:              
		# If you are root user...     
        # Change the root directory to access configuration
        SetUserDir(os.path.abspath('../..'))

    app.run(host='0.0.0.0')
