from pylab import *
import numpy as np
import os, sys
lib_path = os.path.abspath(os.path.join('..','dronelib'))
sys.path.append(lib_path)

from dronelib import *

d=readCsv('logs/kalmantuning1.csv')
#d=readCsv('../../Console/20120806T194404_29785_ardrone2.csv')
          

def unwrap(x):
    return x if x > -90 else x + 360


def veclen(ax, ay, az):
    return math.sqrt(ax**2+ay**2+az**2)

(pitch_kn,pitch_vel_kn,drift_kn,P) = kalman(d)    
    
subplot(3,1,1)
plot([x-5 for x in integrate(d.att_gy)], label="gyro")
angle_from_accel_smoothed=moving_average_list([unwrap(rad2Deg(pitch_a(ax,az))) for (ax,az) in zip(d.att_ax, d.att_az)],8)

plot(angle_from_accel_smoothed,label="accel, future")
plot(d.att_pitch, label="kalman old")
plot(pitch_kn, label="kalman new")

grid()
legend()

subplot(3,1,2)
#plot(d.att_gy_kalman,label="velocity, raw")
velocity_from_gy_smothed=moving_average_list(d.att_gy_kalman,12)
plot(velocity_from_gy_smothed,label="velocity, future")
velocity_from_gy_smothed_lag=moving_average_lag_list(d.att_gy_kalman,12)
plot(velocity_from_gy_smothed_lag,label="velocity, moving_average")
plot(pitch_vel_kn, label="velocity kalman new")

gyNoise=[a-b for a,b in zip(d.att_gy_kalman,velocity_from_gy_smothed)]
#plot(gyNoise,label="Noise in gy")
m=mean(gyNoise)
stddev= sum([ ((x-m)/180*pi)**2 for x in gyNoise])/(len(gyNoise)-1)
print "variance of gy noise: {}".format(stddev)
#
         

grid()
legend()


grid()
legend()

subplot(3,1,3)
#plot(d.att_gy_bias_kalman,label="drift old")
plot(drift_kn, label="drift new")
#plot( [ x[0,0] for  x in P ],label="covariance w")
#plot( [ x[1,1] for  x in P ],label="covariance vel")
#plot( [ x[2,2] for  x in P ],label="covariance drift")

grid()
legend()
show()


