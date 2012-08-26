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

def roll_a(ay,az):
    return atan2(ay,-az)

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


# based on Kalman filter example demo in Python
# by Andrew D. Straw
def kalman(d):

    Q = N.zeros((3,3))# 1e-5 # process variance

    # allocate space for arrays
    xhat = N.zeros((3,1))       # a posteri estimate of x
    P = N.zeros((3,3))          # a posteri error estimate
    xhatminus = N.zeros((3,1))  # a priori estimate of x
    Pminus=N.zeros((3,3))       # a priori error estimate
    K=N.zeros((3,3))               # gain or blending factor


    # linear system

    # matrix of linar system (angle, angle_vel, drift )
    dt = 1.0/200
    F = N.asmatrix(N.eye(3,3))
    F[0,1] = dt
  
    # process noise
    Q[0][0] = 0.0001
    Q[1][1] = 0.01
    Q[2][2] = 0.0000001

#    Q[0][0] = 0.0001
#    Q[1][1] = 0.001
#    Q[2][2] = 0.0000001


    # covariance intial guesses
    P[0][0] = 100.0
    P[1][1] = 100.0
    P[2][2] = 5


    # measurement part    

    # measure matrix
    # accelero, gyro
    H = N.array([[1,0,0],[0,1,1]])

    # estimate of measurement variance, change to see effect
    
    R = N.array([[10,0 ],
                 [0  ,0.5]])

#    R = N.array([[70000,0 ],
#                 [0  ,0.2]])


    kalman_angle=[]
    kalman_anglevel=[]
    kalman_drift=[]
    pitch_from_a=[]
    plist=[]

    for k in range(0, len(d.att_ax)):
        # time update
        xhatminus = F*xhat
        Pminus = F * P * F.transpose() + Q

        # build measurement vector
        pitch = pitch_a(d.att_ax[k], d.att_az[k])
        pitch_from_a.append(rad2Deg(pitch))
        z = N.array( [[pitch], [deg2Rad(d.att_gy[k])]] )

        # measurement error
        y = z - H * xhatminus

        # apply correction
        S = H * Pminus * H.transpose() + R
        K = Pminus * H.transpose() * S.getI()
        xhat = xhatminus + K * y
        P = (N.asmatrix(N.eye(3,3)) - K*H) * Pminus

        
        kalman_angle.append(rad2Deg(xhat[0,0]))
        kalman_anglevel.append(rad2Deg(xhat[1,0]))
        kalman_drift.append(rad2Deg(xhat[2,0]))
        plist.append(P)
        
    return (kalman_angle, kalman_anglevel, kalman_drift,plist)

# this is not linear
def adj_pitch_to_pitch_accel(adjp):
    return (adjp*7000)-250

# based on Kalman filter example demo in Python
# by Andrew D. Straw
def kalman4(d):

    Q = N.zeros((4,4))# 1e-5 # process variance

    # allocate space for arrays
    xhat = N.zeros((4,1))       # a posteri estimate of x
    P = N.zeros((4,4))          # a posteri error estimate
    xhatminus = N.zeros((4,1))  # a priori estimate of x
    Pminus=N.zeros((4,4))       # a priori error estimate
    K=N.zeros((4,4))               # gain or blending factor


    # linear system

    # matrix of linar system (angle, angle_vel, angle_acc drift )
    dt = 1.0/200
    F = N.asmatrix(N.eye(4,4))
    F[0,1] = dt
    F[1,2] = dt
  
    # process noise
    Q[0][0] = 0.0001
    Q[1][1] = 0.0001
    Q[2][2] = 1
    Q[3][3] = 0.0000001

#    Q[0][0] = 0.0001
#    Q[1][1] = 0.001
#    Q[2][2] = 0.0000001


    # covariance intial guesses
    P[0][0] = 10.0
    P[1][1] = 10.0
    P[2][2] = 10.0
    P[3][3] = 5


    # measurement part    

    # measure matrix
    # accelero, gyro
    H = N.array([[1,0,0,0],[0,1,0,1]])

    # estimate of measurement variance, change to see effect
    
    R = N.array([[10,0 ],
                 [0  ,1]])

#    R = N.array([[70000,0 ],
#                 [0  ,0.2]])


    kalman_angle=[]
    kalman_anglevel=[]
    kalman_angleacc=[]
    kalman_drift=[]
    pitch_from_a=[]
    plist=[]

    for k in range(0, len(d.att_ax)):
        # add pitch factor
        
        
        
        
        # time update
        xhatminus = F*xhat
     #   adj_pitch=d.adj_pitch[k]
     #   xhatminus[2]=xhatminus[2]*0.8+deg2Rad(adj_pitch_to_pitch_accel(adj_pitch))*0.2

        
        Pminus = F * P * F.transpose() + Q

        
        # build measurement vector
        pitch = pitch_a(d.att_ax[k], d.att_az[k])
        pitch_from_a.append(rad2Deg(pitch))
        z = N.array( [[pitch], [deg2Rad(d.att_gy[k])]] )

        # measurement error
        y = z - H * xhatminus

        # apply correction
        S = H * Pminus * H.transpose() + R
        K = Pminus * H.transpose() * S.getI()
        xhat = xhatminus + K * y
        P = (N.asmatrix(N.eye(4,4)) - K*H) * Pminus

        
        kalman_angle.append(rad2Deg(xhat[0,0]))
        kalman_anglevel.append(rad2Deg(xhat[1,0]))
        kalman_angleacc.append(rad2Deg(xhat[2,0]))
        kalman_drift.append(rad2Deg(xhat[3,0]))
        plist.append(P)
        
    return (kalman_angle, kalman_anglevel, kalman_angleacc, kalman_drift,plist)

    
def moving_average_single(source,index,halfLen):
    maxIndex=len(source)-1
    if index < halfLen:
        return source[0];
    if index > maxIndex-halfLen:
        return source[maxIndex];

    snippet=source[index-halfLen:index+halfLen]
    return sum(snippet)/len(snippet)

def moving_average_list(source,halfLen):
    return [ moving_average_single(source,i,halfLen) for i in range(len(source))]

def moving_average_lag_single(source,index,halfLen):
    maxIndex=len(source)-1
    if index < halfLen*2:
        return source[0];
    if index > maxIndex:
        return source[maxIndex];

    snippet=source[index-halfLen*2:index]
    return sum(snippet)/len(snippet)

def moving_average_lag_list(source,halfLen):
    return [ moving_average_lag_single(source,i,halfLen) for i in range(len(source))]

