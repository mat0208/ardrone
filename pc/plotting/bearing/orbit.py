from scipy import *
from numpy import *
from pylab import *
from testlib import *
from math import *
from ars import *

(nr,time,flyState,ax,ay,az,gx,gy,gz,
        height_speed,set_h,height,throttle,set_pitch,pitch,
        set_roll,roll,set_yaw,yaw,
        motval_0, motval_1, motval_2,motval_3,
        hor_xv,hor_yv, hor_dt,hor_seqNum,
        xPos,yPos,
        targetXVel,targetYVel,
        targetPitch, targetRoll,
        adj_pitch, adj_roll, adj_yaw, adj_h)=read_fullDroneLog('logs/pose_control.csv')



plot(xPos)
plot(yPos)
show()
