from scipy import *
from numpy import *
from pylab import *
from testlib import *

d=readCsv('logs/drone_1_1.csv')

a=subplot(3,1,1)
a.plot(d.ax, label="ax")
a.plot(d.ay, label="ay")
a.plot(d.az, label="az")
legend()

g=subplot(3,1,2)
g.plot(d.gx, label="gx")
g.plot(d.gy, label="gy")
g.plot(d.gz, label="gz")
legend()


m=subplot(3,1,3)
m.plot(d.mx, label="mx")
m.plot(d.my, label="my")
m.plot(d.mz, label="mz")
legend()

show()





    
