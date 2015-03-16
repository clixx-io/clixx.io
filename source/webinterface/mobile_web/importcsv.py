import csv

di = {}

#"First" : {"1":{"Text":"Subdiv",
#                     "Page":"Sometext"}
#                ,"2":"AnotherDiv"}}

def add_item(majorgroup, subgroup, txtTitle, itemid):

    if majorgroup not in di.keys():
        print '%s ' % (majorgroup)
        di[majorgroup] = {}

    if subgroup not in di[majorgroup].keys():
        di[majorgroup][subgroup] = {} 
        print ' + %s ' % (subgroup)

    di[majorgroup][subgroup][itemid] = {"Text":txtTitle,"Page":itemid}  
    
    return

def import_groupsfromcsv():

    ifile  = open('/home/dlyon/webshop_datautils/webpmf.csv', "rb")
    reader = csv.reader(ifile)

    rownum = 0
    for row in reader:
        # Save header row.
        if rownum == 0:
            header = row
        else:
            colnum = 0
            for col in row:
                if header[colnum] == 'WNAME':   
                    # print '%-8s: %s-%s' % (header[colnum], col[:col.find('>',2)], col[col.find('>',2):])
                    l1 = col[1:col.find('>',2)]
                    l2 = col[col.find('>',2)+1:]

                    add_item(l1, l2, row[0],row[0]+'.html')

                    # print '%s %s' % (l1,l2)

                colnum += 1
            
        rownum += 1

    ifile.close()

def output_categories():

    ofile  = open('pages.csv', "wb")
    writer = csv.writer(ofile, delimiter=',', quotechar='"', quoting=csv.QUOTE_ALL)

    for majorgroup in di.keys():
        for minorgroup in di[majorgroup].keys():
            for itm in di[majorgroup][minorgroup].keys():

                writer.writerow((majorgroup,minorgroup,di[majorgroup][minorgroup][itm]['Text'],di[majorgroup][minorgroup][itm]['Page']))
 
    ofile.close()



if __name__ == "__main__":
    
    import_groupsfromcsv()
    output_categories()
