from pylab import *
from testlib import *


d=readCsv('logs/drone_2_1.csv')


yaw=integrate(d.gz)

subplot(2,1,1)
plot(yaw,label = 'yaw')
grid()
legend()

subplot(2,1,2)
plot(d.gz,label = 'gz')
grid()
legend()

show()
