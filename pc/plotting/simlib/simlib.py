

from functools import partial
import numpy as N
from pylab import *
from scipy.integrate import odeint




# model taken from "Design and Control of Quadrotors with Application to Autonomous Flying" by Samir Bouabdallah 2007

# state
# roll roll_dot pitch pitch_dot yaw yaw_dot z z_dot x x_dot y y_dot


def motorSpeedsToUmap(omegas,b,d):
    'converts the motor speed to to thrust, roll influence, pitch influes and yaw influence'
    o1s=omegas[0]**2
    o2s=omegas[1]**2
    o3s=omegas[2]**2
    o4s=omegas[3]**2

    # todo: are the motors indices corresponding to the other model
    return [
        b*( o1s+o2s+o3s+o4s),
        b*( o1s-o2s-o3s+o4s),
        b*(-o1s-o2s+o3s+o4s),
        d*-(+o1s-o2s+o3s-o4s)
        ] 
    


def nullController(x, thrust=0):
    return [thrust, thrust, thrust ,thrust]


def motorOutputToAngularSpeed(mo):
    return mo*120.0+40.0


class SimParams:
    def __init__(self):
        # inertia moments
        self.I_xx = 0.0075
        self.I_yy = 0.0075
        self.I_zz = 0.0260
        
        self.J_r = 0.00006    # rotor inertia
       
        
        self.d = 0.00000010   # drag coefficient
        self.b = 0.0000313    # thrust coefficient

# known
        self.arm_length = 0.18 # distance of prop center to center of grav
        self.g = 9.81          # gravity accel.
        self.m = 0.470         # overall mass




def dummyNoiseFunc(t):
    return N.zeros(12)


    
def droneFunc(x, t, simParams, controller = nullController, noiseFunc=dummyNoiseFunc ):
    p=simParams
    
    a1 = (p.I_yy - p.I_zz) / p.I_xx
    a2 = p.J_r / p.I_xx
    a3 = (p.I_zz - p.I_xx) / p.I_yy
    a4 = p.J_r / p.I_yy
    a5 = (p.I_xx - p.I_yy) / p.I_zz
    b1 = p.arm_length/p.I_yy
    b2 = p.arm_length/p.I_yy
    b3 = 1.0 / p.I_zz

    inputs=controller(x,t)

    motorSpeeds=[motorOutputToAngularSpeed(mo) for mo in inputs ]
    
    uValues=motorSpeedsToUmap(motorSpeeds, b=p.b, d=p.d)
    [U1,U2,U3,U4] = uValues

    [roll,roll_dot,pitch,pitch_dot,yaw,yaw_dot,z,z_dot,x,x_dot,y,y_dot]=x

    # directions
    u_x = cos(roll)*sin(pitch)*cos(yaw)+ sin(roll)*sin(yaw)
    u_y = cos(roll)*sin(pitch)*sin(yaw)- sin(roll)*cos(yaw)

    deriv= np.array([
        roll_dot,
        pitch_dot*yaw_dot*a1 + b1*U2,
        pitch_dot,
        roll_dot*yaw_dot*a3   + b2*U3,
        yaw_dot,
        pitch_dot*roll_dot*a5 + b3*U4,
        z_dot,
        p.g- cos(roll) * cos(pitch)/p.m*U1,
        x_dot,
        u_x * U1/p.m ,
        y_dot,
        u_y * U2/p.m
      ])  


    noise=noiseFunc(t)
    
    return deriv+noise

def angleDroneFunc(x, t, simParams, controller = nullController, noiseFunc=dummyNoiseFunc ):
    p=simParams
    
    a1 = (p.I_yy - p.I_zz) / p.I_xx
    a3 = (p.I_zz - p.I_xx) / p.I_yy
    a5 = (p.I_xx - p.I_yy) / p.I_zz
    b1 = p.arm_length/p.I_yy
    b2 = p.arm_length/p.I_yy
    b3 = 1.0 / p.I_zz

    inputs=controller(x,t)

    motorSpeeds=[motorOutputToAngularSpeed(mo) for mo in inputs ]
    
    uValues=motorSpeedsToUmap(motorSpeeds, b=p.b, d=p.d)
    [U1,U2,U3,U4] = uValues

    [roll,roll_dot,pitch,pitch_dot,yaw,yaw_dot]=x

    deriv= np.array([
        roll_dot,
        pitch_dot*yaw_dot*a1 + b1*U2,
        pitch_dot,
        roll_dot*yaw_dot*a3   + b2*U3,
        yaw_dot,
        pitch_dot*roll_dot*a5 + b3*U4,
      ])  


    noise=noiseFunc(t)[0:6]
    
    return deriv+noise
