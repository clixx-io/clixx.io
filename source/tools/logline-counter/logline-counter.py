#!/usr/bin/python
import os.path
import glob
import platform
import time
import logging
import datetime

if __name__ == "__main__":

    from optparse import OptionParser

    usage = "usage: %prog [options] arg"
    parser = OptionParser(usage)
    parser.add_option("-t", "--trace", dest="trace",action="store_true",
                      help="Generate trace data")
    parser.add_option("-f", "--file", dest="filename",
                      help="read data from FILENAME")
    parser.add_option("-d", "--directories", dest="directories",
                      help="Specify directories")
    parser.add_option("-k", "--keep", dest="keep",action="store_true",
                      help="Keep Input files. Dont delete them")

    (options, args) = parser.parse_args()

    txtfiles = []

    print "logline counter"

    logging.basicConfig(format='%(asctime)s %(message)s',filename='logline-counter.log',level=logging.DEBUG)
    
    log = logging.getLogger("logline-counter")
    log.setLevel(logging.DEBUG) #set verbosity to show all messages of severity >= DEBUG
    log.info("Program startup")    

    purgeafterprocess = options.keep == None
       
    if not options.filename is None:
        # Processing a single file, implies debugging, so dont delete it
        txtfiles = [options.filename]
        purgeafterprocess = False
    else:
        purgeafterprocess = True

    # purgeafterprocess = False
    scandirectories = ["/home/user/dir1",
                       "/home/user/dir2"
                      ]

    try:

        while (1):
    
            try:

                # Remove files more than two days old
                if not (datetime.datetime.today().weekday() in [0,1]):

                    twodays_ago = time.time() - 60*60*24*2
                    for scandir in scandirectories:
                        pdffiles = glob.glob(os.path.join(scandir,"*"))
                        for pdffile in pdffiles:
                            fileCreation = os.path.getctime(pdffile)
                            if fileCreation < twodays_ago:
                                log.info("Cleaning up %s as it is more than two days old" % pdffile)
                                os.remove(pdffile)

                time.sleep(60)             # wait for a second
    
            except Exception, e:
                log.exception(e)

    except (KeyboardInterrupt, SystemExit):
        pass
        # report error and proceed

    log.info("Program shutdown")    

