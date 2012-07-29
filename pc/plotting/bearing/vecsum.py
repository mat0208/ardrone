from scipy import *
from numpy import *
from pylab import *
from testlib import *
from math import *

(ax,ay,az,gx,gy,gz,mx,my,mz)=read_droneAngs('logs/drone1_1.csv')

ax=[ a *-1 for a in ax]
az=[ a *-1 for a in az]


ls=[sqrt(x*x+z*z)/9.81 for (x,z) in zip(ax,az)]

plot(ls)
show()
