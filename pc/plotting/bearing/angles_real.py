from scipy import *
from numpy import *
from pylab import *
from testlib import *
from math import *
from ars import *


d=readCsv('logs/drone_2_1.csv')


(gxi,gyi,gzi)=(integrate(d.gx),integrate(d.gy),integrate(d.gz))

ars_Init(Q_angle=0.001, Q_gyro=0.003, R_angle=0.69)


angles_g=[]
angles_a=[]
angles_kal=[]
bias=[]

for i in range(len(d.ax)):
    pitch_f_a=pitch_a(d.ax[i],d.az[i])
    angles_a.append(rad2Deg(pitch_f_a))
    angles_g.append(gyi[i])
    ars_predict(deg2Rad(d.gy[i]),0.005)
    ang_kal=ars_update(pitch_f_a)
    angles_kal.append(rad2Deg(ang_kal))
    bias.append(b())
    
subplot(2,1,1)
plot(angles_g,label="aus g")
plot(angles_a,label="aus a")
plot(angles_kal,label="aus kal")
grid()
legend()

subplot(2,1,2)
plot(bias,label="bias")
grid()
legend()
show()


