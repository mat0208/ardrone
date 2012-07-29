from pylab import *
from testlib import *

data=readCsv('logs/pose_control.csv')

plot(data.xPos,data.yPos)

show()
