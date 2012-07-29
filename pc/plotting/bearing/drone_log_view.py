from pylab import *
from testlib import *

d=readCsv('logs/pose_control.csv')

    
subplot(3,1,1)
plot(d.roll,label="roll")
plot([x*100 for x in d.adj_roll],label="adj_roll")

grid()
legend()

subplot(3,1,2)
plot(d.pitch,label="pitch")
plot([x*100 for x in d.adj_pitch],label="adj_pitch")

grid()
legend()

subplot(3,1,3)
plot(d.motval_0,label="mot 0")
plot(d.motval_1,label="mot 1")
plot(d.motval_2,label="mot 2")
plot(d.motval_3,label="mot 3")
grid()
legend()
show()


