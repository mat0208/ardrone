import numpy as N
from math import *
from re import *

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

def read_droneAngs(filename):
    ax=[]
    ay=[]
    az=[]
    gx=[]
    gy=[]
    gz=[]
    mx=[]
    my=[]
    mz=[]
    
    arrs=[ax,ay,az,gx,gy,gz,mx,my,mz]
        

    first=True
    for line in open(filename, 'r'):
        if first:
            first=False
        else:
            fields = line.strip().split(',')
            for i, number in enumerate(fields):
                arrs[i].append(float(number))

    return arrs

def read_seqDroneAngs(filename):
    seq=[]
    ax=[]
    ay=[]
    az=[]
    gx=[]
    gy=[]
    gz=[]
    mx=[]
    my=[]
    mz=[]
    
    arrs=[seq,gx,gy,gz,ax,ay,az,mx,my,mz]
        

    first=True
    for line in open(filename, 'r'):
        if first:
            first=False
        else:
            fields = line.strip().split(',')
            for i, number in enumerate(fields):
                arrs[i].append(float(number))

    return arrs

def read_rawDroneAngs(filename):
    ax=[]
    ay=[]
    az=[]
    
    arrs=[ax,ay,az]
        

    first=True
    for line in open(filename, 'r'):
        if first:
            first=False
        else:
            fields = line.strip().split(',')
            for i, number in enumerate(fields):
                arrs[i].append(float(number))

    return arrs



def read_fullDroneLog(filename):
    nr=[]
    time=[]
    flyState=[]
    ax=[]
    ay=[]
    az=[]
    gx=[]
    gy=[]
    gz=[]
    height_speed=[]
    set_h=[]
    height=[]
    throttle=[]
    set_pitch=[]
    pitch=[]
    set_roll=[]
    roll=[]
    set_yaw=[]
    yaw=[]
    motval_0=[]
    motval_1=[]
    motval_2=[]
    motval_3=[]
    hor_xv=[]
    hor_yv=[]
    hor_dt=[]
    hor_seqNum=[]
    xPos=[]
    yPos=[]
    targetXVel=[]
    targetYVel=[]
    targetPitch=[]
    targetRoll=[]
    adj_pitch=[]
    adj_roll=[]
    adj_yaw=[]
    adj_h=[]
    

    
    arrs=[nr,time,flyState,ax,ay,az,gx,gy,gz,
        height_speed,set_h,height,throttle,set_pitch,pitch,
        set_roll,roll,set_yaw,yaw,
        motval_0, motval_1, motval_2,motval_3,
        hor_xv,hor_yv, hor_dt,hor_seqNum,
        xPos,yPos,
        targetXVel,targetYVel,
        targetPitch, targetRoll,
        adj_pitch, adj_roll, adj_yaw, adj_h
    ]
        

    first=True
    for line in open(filename, 'r'):
        if first:
            first=False
        else:
            fields = line.strip().split(',')
            for i, number in enumerate(fields):
                if number!="":
                  try:
                      arrs[i].append(float(number))
                  except ValueError:
                      arrs[i].append(0);
                      

    return arrs
