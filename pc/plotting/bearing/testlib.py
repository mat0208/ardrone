import numpy as N
from math import *
import re
import csv

def integrate(ts):
    sum=0
    result=[]
    for x in ts:
        sum+=x/200.0
        result.append(sum)
    return result

def differentiate(ts):
    oldVal=ts[0]
    result=[]
    for x in ts:
        a=x-oldVal
        oldVal=x
        result.append(a)
    return result




def pitch_a(ax,az):
    return atan2(ax,-az)

def deg2Rad(d):
    return d/180*pi


def rad2Deg(d):
    return d*180/pi

def readCsv(filename):

    class ListCollection:
        def names(self):
            return self.__dict__.keys()

    def removeUnits(row):
        return [ re.sub('\[.*\]', '', text).strip() for text in row ]
    
    lc=ListCollection()
    import csv

    usedNames=dict()

    csvFile  = open(filename, 'rb')
    reader = csv.reader(csvFile)

    rownum = 0

    for row in reader:
        if rownum == 0:
            header = removeUnits(row)
            for head in header:
                if head in usedNames:
                    raise Exception('Duplicate header \'{}\''.format(head))
                lc.__dict__[head]=[]
                usedNames[head]=1
        else:
            colnum = 0
            for col in row:
                colname=header[colnum]
                lc.__dict__[colname].append(float(col))
                colnum += 1
        rownum += 1

    csvFile.close
    return lc





