import os
import platform
import argparse

from mako import exceptions    
from mako.lookup import TemplateLookup
from mako.template import Template

import csv
from ftplib import FTP

import logging
#logging.basicConfig(filename='mobile.log',level=logging.DEBUG)
logging.basicConfig(level=logging.DEBUG)

template_root = "templates"

di = {}
groupkeys = {}

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

    kn = '/' + majorgroup + '/' + subgroup 
    if kn not in groupkeys.keys():
        groupkeys[kn] = 'g' + str(len(groupkeys)+1)
    
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

    for gk in groupkeys.keys():
        print groupkeys[gk], gk
        
    return

if __name__ == "__main__":

    load_tableinfo()

    generate_main()

    parser = argparse.ArgumentParser()
    parser.add_argument('-t', action='store_true',help='Transfer the page to the website')
    args = parser.parse_args()

    generate_product_pages()

    ci = {"CompanyName" : "clixx.io"}

    t = Template(filename='templates/jqueryexample.mako')
    open('mobile.html','w').write(t.render(di=di,ci=ci))

    # Upload to the website if that option was specified
    if vars(args)['t']:
        upload()

