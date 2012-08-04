from scipy import *
from numpy import *
from pylab import *
import os, sys
lib_path = os.path.abspath(os.path.join('..','dronelib'))
sys.path.append(lib_path)

from dronelib import *
from math import *

d=readCsv('logs/drone_1_1.csv')

ax=[ a *-1 for a in d.ax]
az=[ a *-1 for a in d.az]


ls=[sqrt(x*x+z*z)/9.81 for (x,z) in zip(ax,az)]

plot(ls)
show()
