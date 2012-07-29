from scipy import *
from numpy import *
from pylab import *
from testlib import *
from math import *
from ars import *

(seq,gx,gy,gz,ax,ay,az,mx,my,mz)=read_seqDroneAngs('logs/drone_2_1.csv')


yaw=integrate(gz)

subplot(2,1,1)
plot(yaw,label = 'yaw')
grid()
legend()

subplot(2,1,2)
plot(gz,label = 'gz')
grid()
legend()

show()
