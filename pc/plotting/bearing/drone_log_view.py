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

    
subplot(3,1,1)
plot(roll,label="roll")
plot([x*100 for x in adj_roll],label="adj_roll")

grid()
legend()

subplot(3,1,2)
plot(pitch,label="pitch")
plot([x*100 for x in adj_pitch],label="adj_pitch")

grid()
legend()

subplot(3,1,3)
plot(motval_0,label="mot 0")
plot(motval_1,label="mot 1")
plot(motval_2,label="mot 2")
plot(motval_3,label="mot 3")
grid()
legend()
show()


