import os
import platform

from mako import exceptions    
from mako.lookup import TemplateLookup
from mako.template import Template

import csv

template_root = "templates"

di = {}

def make_page(filename):
    
    template_lookup = TemplateLookup(directories=[template_root])    
    
    try:
        return template_lookup.get_template(filename)
    except exceptions.TopLevelLookupException:
        raise httpclient.HTTPError(404)

def add_item(majorgroup, subgroup, txtTitle, itemid):

    if majorgroup not in di.keys():
        print '%s ' % (majorgroup)
        di[majorgroup] = {}

    if subgroup not in di[majorgroup].keys():
        di[majorgroup][subgroup] = {} 
        print ' + %s ' % (subgroup)

    di[majorgroup][subgroup][itemid] = {"Text":txtTitle,"Page":itemid}  
    
    return


def load_tableinfo():

    ifile  = open('pages.csv', "rb")
    reader = csv.reader(ifile)

    rownum = 0
    for row in reader:
        # Save header row.
        if rownum == 0:
            header = row
        else:
            add_item(row[0], row[1], row[2], row[3])            
        rownum += 1

    ifile.close()

    return

def generate_main():

    return

def generate_product_pages():

    return

if __name__ == "__main__":

    load_tableinfo()
     
    generate_main()

    generate_product_pages()

    ci = {"CompanyName"   : "Clixx.io",
          "SystemTitle"   : "Documentation for Clixx.io",
          "SystemSummary" : "Welcome to the clixx.io documentation.", }

    t = Template(filename='templates/jqueryexample.mako')
    open('m.html','w').write(t.render(di=di,ci=ci))

    # print Template("hello ${data}!").render(data="world")
