from pylab import *
import numpy as np
import os, sys

lib_path = os.path.abspath(os.path.join('..','dronelib'))
sys.path.append(lib_path)

from dronelib import *

#d=readCsv('logs/20120804T214742_18593_ardrone2.csv')
#d=readCsv('../../Console/20120812T202241_8068_ardrone2.csv')
d=readCsv('../../Console/20120813T001753_17882_ardrone2.csv')
          

 
dt=[x* 1000 for x in differentiate(d.att_ts[200:])]
subplot(2,1,1)
plot(dt,label='dt')

print np.std(dt)

grid()
legend()

subplot(2,1,2)
fft= abs(np.fft.rfft(dt))**2
points=len(fft)-1
fft=fft[1:points+1]
freqs=arange(0,points,1.0)* ( 200.0/len(dt))
plot(freqs,fft)


grid()
legend()



grid()
legend()
show()


