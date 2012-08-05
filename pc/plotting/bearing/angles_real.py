from scipy import *
from numpy import *
from pylab import *
import os, sys
lib_path = os.path.abspath(os.path.join('..','dronelib'))
sys.path.append(lib_path)

from dronelib import *
from math import *
from ars import *





#d=readCsv('logs/20120804T201858_16753_ardrone2.csv')

d=readCsv('logs/20120804T214742_18593_ardrone2.csv')

(gxi,gyi,gzi)=(integrate(d.att_gx),integrate(d.att_gy),integrate(d.att_gz))

ars_Init(Q_angle=0.001, Q_gyro=0.003, R_angle=0.69)


angles_g=[]
angles_a=[]
angles_drone=[]
angles_kal=[]
bias=[]



for i in range(len(d.att_ax)):
    pitch_f_a=pitch_a(d.att_ax[i],d.att_az[i])
    angles_a.append(rad2Deg(pitch_f_a))
    angles_g.append(gyi[i])
    angles_drone.append(d.att_pitch[i])
    ars_predict(deg2Rad(d.att_gy[i]),0.005)
    ang_kal=ars_update(pitch_f_a)
    angles_kal.append(rad2Deg(ang_kal))
    bias.append(b())

shift=28
print "shift is {} s\n".format(shift/200.0)

smoothed=moving_average_list(angles_a,8)
cor=correlate(smoothed,angles_drone, mode='full')

subplot(2,1,1)
plot([x-17 for x in angles_g][0:-shift],label="aus g")
#plot(angles_a[shift:],label="aus a")
plot(angles_kal[0:-shift],label="aus kal")
plot(angles_drone[0:-shift],label="aus drohne")
plot(mov_a_list(angles_a[shift:],8),label="a geglaettet")
grid()
legend()

subplot(2,1,2)
#plot(bias,label="bias")
corlenh=len(cor)/2
plot(range(-corlenh,len(cor)-corlenh),cor,label="corr")

grid()
legend()
show()



