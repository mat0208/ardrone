from pylab import *
import numpy as np
import os, sys
lib_path = os.path.abspath(os.path.join('..','dronelib'))
sys.path.append(lib_path)

from dronelib import *

#d=readCsv('logs/kalman_new_2.csv')
d=readCsv('../../Console/20120824T201357_27123_ardrone2.csv')
          

def unwrap(x):
    return x if x > -90 else x + 360


def veclen(ax, ay, az):
    return math.sqrt(ax**2+ay**2+az**2)

def timeSeg(x):
        return x[500:]
 
roll_f_a=[ rad2Deg(roll_a(ax,az)) for (ax,az) in zip(d.att_ax,d.att_az)]
roll_f_a_s=moving_average_list(roll_f_a,12)

subplot(4,1,1)
#plot([x for x in integrate(d.att_gy)], label="gyro")
#plot(roll_f_a_s,label='roll from acc')


plot(timeSeg(d.targetRoll), label="target roll ")
plot(timeSeg(d.att_roll), label="roll  (kalman)")
plot([x*100 for x in timeSeg(d.adj_roll)],label="adj_roll")
plot(timeSeg(d.targetRollVel), label="target roll vel")


grid()
legend()

subplot(4,1,2)
#plot([x for x in integrate(d.att_gy)], label="gyro")
#plot(roll_f_a_s,label='roll from acc')



plot(timeSeg(d.targetRollVel), label="target roll vel")
plot(timeSeg(d.att_roll_vel_kalman), label="roll vel (kalman)")
plot(timeSeg(d.att_gx), label="roll vel (gyro)")
plot(timeSeg(d.att_filtered_gx), label="roll vel (lp)")
plot([x*100 for x in timeSeg(d.adj_roll)],label="adj_roll")

grid()
legend()

subplot(4,1,3)
#plot([x for x in d.adj_roll],label="adj_roll")
#plot(d.att_h,label="height")
         
fft= abs(np.fft.rfft(d.adj_roll))**2
points=len(fft)-1
fft=fft[1:points+1]
freqs=arange(0,points,1.0)* ( 200.0/len(d.adj_roll))
plot(freqs,fft, label="fft adj roll")


grid()
legend()


grid()
legend()

subplot(4,1,4)
plot(timeSeg(d.motval_0),label="motval 0 vl")
plot(timeSeg(d.motval_1),label="motval 1 vr")
plot(timeSeg(d.motval_2),label="motval 2 hr")
plot(timeSeg(d.motval_3),label="motval 3 hl")

grid()
legend()
show()


