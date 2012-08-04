from pylab import *
import os, sys
lib_path = os.path.abspath(os.path.join('..','dronelib'))
sys.path.append(lib_path)

from dronelib import *

data=readCsv('logs/pose_control.csv')

plot(data.xPos,data.yPos)

show()
