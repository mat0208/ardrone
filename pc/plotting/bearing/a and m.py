from scipy import *
from numpy import *
from pylab import *
from testlib import *

d=readCsv('logs/drone_1_1.csv')


plot(d.ax)
plot(  [ (a+71.5)/13.5 for a in d.my] )
plot( [ a * -0.015/20 for a in integrate(d.gy)  ])
show()

