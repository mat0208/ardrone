from scipy import *
from numpy import *
from pylab import *

import os, sys
lib_path = os.path.abspath(os.path join('..','dronelib'))
sys.path.append(lib_path)

from dronelib import *

d=readCsv('logs/drone_1_1.csv')


plot(d.ax)
plot(  [ (a+71.5)/13.5 for a in d.my] )
plot( [ a * -0.015/20 for a in integrate(d.gy)  ])
show()

