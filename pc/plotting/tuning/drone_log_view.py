from pylab import *
import numpy as np
import os, sys
lib_path = os.path.abspath(os.path.join('..','dronelib'))
sys.path.append(lib_path)

from dronelib import *

d=readCsv('../../Console/20120804T201858_16753_ardrone2.csv')

def unwrap(x):
    return x if x > -90 else x + 360


def veclen(ax, ay, az):
    return math.sqrt(ax**2+ay**2+az**2)

(angle,anglevel,drift) = kalman(d)    
    
subplot(2,1,1)
plot(d.att_pitch, label="pitch")
#plot([x*100 for x in d.adj_pitch],label="adj_pitch")
plot(integrate(d.att_gy), label="pitch_g")
plot([unwrap(rad2Deg(pitch_a(ax,az))) for (ax,az) in zip(d.att_ax, d.att_az)],label="pitch_a")
plot(angle, label="angle")
plot([veclen(ax, ay, az) for (ax, ay, az) in zip(d.att_ax, d.att_ay, d.att_az)], label="veclen")
grid()
legend()

subplot(2,1,2)
plot(anglevel, label="anglevel")
plot(drift, label="drift")
grid()
legend()
#plot(differentiate(d.att_ts), label="dt")
#plot([x*10 for x in d.att_magx], label="magx")
#plot([x*1 for x in d.att_gx], label="gx")
#plot(d.att_magy, label="magy")
#plot(d.att_magz, label="magz")



#grid()
#legend()

#subplot(4,1,3)
#plot(d.att_gy_kalman,label="gy_kalman")
#plot(d.att_gy_bias_kalman,label="gy_bias_kalman")
#plot([x*1000 for x in d.adj_pitch],label="adj_pitch")

##fftData=abs(np.fft.rfft(d.att_pitch))**2
##chunk=len(d.att_gy_kalman)
##freqs=[200.0/chunk*i for i in range(1,(chunk)/2+1)]
##plot(freqs, fftData[1:], label="fft")
##which = fftData[1:].argmax() + 1
##thefreq = (which+1)*200/chunk
##print(thefreq)
#ampl = fftData[which]
#if ampl > 100*chunk*chunk:
#    print "The freq is %f Hz (ampl %f)." % (thefreq,ampl)
#    fftData*=1.0/max(ampl,100*chunk*chunk)	

#plot([x*100 for x in d.targetPitch],label="targetPitch")

##grid()
##legend()
##
##subplot(4,1,4)
##plot(d.motval_0,label="mot 0")
##plot(d.motval_1,label="mot 1")
##plot(d.motval_2,label="mot 2")
##plot(d.motval_3,label="mot 3")
##grid()
##legend()
show()


