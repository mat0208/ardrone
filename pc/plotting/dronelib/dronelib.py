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
        return [ re.sub('\[.*\]', '', text).strip().replace('.','_') for text in row ]
    
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


# Kalman filter example demo in Python

# A Python implementation of the example given in pages 11-15 of "An
# Introduction to the Kalman Filter" by Greg Welch and Gary Bishop,
# University of North Carolina at Chapel Hill, Department of Computer
# Science, TR 95-041,
# http://www.cs.unc.edu/~welch/kalman/kalmanIntro.html

# by Andrew D. Straw
def kalman(d):

    Q = N.zeros((3,3))# 1e-5 # process variance

    # allocate space for arrays
    xhat = N.zeros((3,1))       # a posteri estimate of x
    P = N.zeros((3,3))          # a posteri error estimate
    xhatminus = N.zeros((3,1))  # a priori estimate of x
    Pminus=N.zeros((3,3))       # a priori error estimate
    K=N.zeros((3,3))               # gain or blending factor

    #R = 0.1**2 # estimate of measurement variance, change to see effect
    R = N.array([[0.4,0],[0,0.5]])

    # intial guesses
    P[0][0] = 1000.0
    P[1][1] = 1000.0
    P[2][2] = 1000.0


    Q[0][0] = 0.02
    Q[1][1] = 0.01
    Q[2][2] = 0.01

    F = N.asmatrix(N.eye(3,3))

    dt = 1/200

    F[1,2] = dt

    H = N.array([[1,0,0],[0,1,1]])

    kalman_angle=[]
    kalman_anglevel=[]
    kalman_drift=[]
    pitch_from_a=[]

    for k in range(0, len(d.att_ax)):
        # time update
        xhatminus = F*xhat
        Pminus = F * P * F.transpose() + Q

        pitch = pitch_a(d.att_ax[k], d.att_az[k])
        pitch_from_a.append(rad2Deg(pitch))
        z = N.array([pitch, deg2Rad(d.att_gy[k])])
        
        y = z - H * xhatminus
        S = H * Pminus * H.transpose() + R
        K = Pminus * H.transpose() * S.getI()
        xhat = xhatminus + K * y
        P = (N.asmatrix(N.eye(3,3)) - K*H) * Pminus
        kalman_angle.append(rad2Deg(xhat[0,0]))
        kalman_anglevel.append(rad2Deg(xhat[1,0]))
        kalman_drift.append(xhat[2,0])
        
    return (kalman_angle, kalman_anglevel, kalman_drift)
