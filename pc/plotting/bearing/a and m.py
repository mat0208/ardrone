from scipy import *
from numpy import *
from pylab import *
from testlib import *

(ax,ay,az,gx,gy,gz,mx,my,mz)=read_droneAngs('logs/drone_1_1.csv')


plot(ax)
plot(  [ (a+71.5)/13.5 for a in my] )
plot( [ a * -0.015/20 for a in integrate(gy)  ])
show()

