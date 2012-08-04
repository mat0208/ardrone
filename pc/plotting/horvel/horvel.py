from testlib import *
from pylab import *

def singleMedian(arr,i,halfWindow):
    if(i < halfWindow or i>=len(arr)-halfWindow):
        return arr[i]


    return median(arr[i-halfWindow:i+halfWindow])

def medianFilter(arr,halfWindow):
    return [ singleMedian(arr,i,halfWindow) for i in range(len(arr)) ]


d=readCsv('logs/horvel.csv')
a=d.names()
a.sort()
print a

subplot(3,1,1)
plot(d.hor_velocities_seqNum, medianFilter(d.hor_velocities_xv,1), label='xv')
plot(d.hor_velocities_seqNum, medianFilter(d.hor_velocities_yv,1), label='yv')
legend()
grid()


subplot(3,1,2)
plot(d.hor_velocities_seqNum,d.att_roll, label='roll')
plot(d.hor_velocities_seqNum,d.att_pitch, label='pitch')
plot(d.hor_velocities_seqNum,[a*10 for a in d.hor_velocities_dt],label='duration (100) ms');
     
legend()
grid()



sp=subplot(3,1,3)
cm = plt.get_cmap('winter')
downSample=10
pos_x=integrate(d.hor_velocities_xv)[::downSample]
pos_y=integrate(d.hor_velocities_yv)[::downSample]

nPoints=len(pos_x)
sp.set_color_cycle([cm(1.*i/(nPoints-1)) for i in range(nPoints-1)])


for i in range(nPoints-1):
    plot(pos_x[i:i+2],pos_y[i:i+2])
    if(i*downSample % 1000 == 0):
        text(pos_x[i],pos_y[i],d.hor_velocities_seqNum[i*downSample])
grid()

show()


