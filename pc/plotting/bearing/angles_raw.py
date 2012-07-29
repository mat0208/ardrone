from scipy import *
from numpy import *
from pylab import *
from testlib import *
from math import *

(ax,ay,az)=read_rawDroneAngs('logs/raw.csv')

ay=[ (x+32-2048)/512.0*10 for x in ay]
ax=[ (x-10-2048)/512.0*10 for x in ax]
az=[ (x-12-2048)/512.0*10 for x in az]

subplot(3,1,1)
plot(ax,label='ax')
plot(ay,label='ay')
plot(az,label='az')
legend()
grid()


subplot(3,1,2)

ang= [-atan2(x,z)/pi*180 for x,z in zip(ax,az)]

ang_x = [ -asin(x/10)/pi*180 for x in ax ]

plot(ang,label="angle tan")
plot(ang_x,label="angle x")
legend()
grid()

subplot(3,1,3)

l= [sqrt(x*x+z*z) for x,z in zip(ax,az)]
plot(l)

show()

