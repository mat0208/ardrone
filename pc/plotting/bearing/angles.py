from scipy import *
from numpy import *
from pylab import *
from testlib import *

(ax,ay,az,gx,gy,gz,mx,my,mz)=read_droneAngs('logs/drone_1_1.csv')

a=subplot(3,1,1)
a.plot(ax, label="ax")
a.plot(ay, label="ay")
a.plot(az, label="az")
legend()

g=subplot(3,1,2)
g.plot(gx, label="gx")
g.plot(gy, label="gy")
g.plot(gz, label="gz")
legend()


m=subplot(3,1,3)
m.plot(mx, label="mx")
m.plot(my, label="my")
m.plot(mz, label="mz")
legend()

show()





    
