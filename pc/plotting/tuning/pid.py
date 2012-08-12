from pylab import *
import numpy as np
import os, sys
lib_path = os.path.abspath(os.path.join('..','dronelib'))
sys.path.append(lib_path)

from dronelib import *

#d=readCsv('logs/20120804T214742_18593_ardrone2.csv')
d=readCsv('../../Console/20120806T202231_30243_ardrone2.csv')
          

def unwrap(x):
    return x if x > -90 else x + 360


def veclen(ax, ay, az):
    return math.sqrt(ax**2+ay**2+az**2)

 
    
subplot(3,1,1)
plot([x-5 for x in integrate(d.att_gy)], label="gyro")


plot(d.att_pitch, label="kalman old")
plot([x*100 for x in d.adj_pitch],label="adj_pitch")

grid()
legend()

subplot(3,1,2)
plot([x*20 for x in d.adj_pitch],label="adj_pitch")
plot(d.att_h,label="height")
         

grid()
legend()


grid()
legend()

subplot(3,1,3)
plot(d.motval_0,label="motval 0")
plot(d.motval_1,label="motval 0")
plot(d.motval_2,label="motval 0")
plot(d.motval_3,label="motval 0")

grid()
legend()
show()


