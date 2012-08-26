from pylab import *
import numpy as np
from scipy.signal import kaiserord, lfilter, firwin, freqz
import os, sys
lib_path = os.path.abspath(os.path.join('..','dronelib'))
sys.path.append(lib_path)

from dronelib import *

#d=readCsv('logs/kalmantuning1.csv')
d=readCsv('../../Console/20120824T201357_27123_ardrone2.csv')
          

def unwrap(x):
    return x if x > -90 else x + 360


def veclen(ax, ay, az):
    return math.sqrt(ax**2+ay**2+az**2)

def lowpass(sig):
      sample_rate=200
      # The Nyquist rate of the signal.
      nyq_rate = sample_rate / 2.0
      # The desired width of the transition from pass to stop,
      # relative to the Nyquist rate.  We'll design the filter
      # with a 10 Hz transition width.
      width = 5.0/nyq_rate

      # The desired attenuation in the stop band, in dB.
      ripple_db = 10.0
      # Compute the order and Kaiser parameter for the FIR filter.
      N, beta = kaiserord(ripple_db, width)
      print "N is " , N
      # The cutoff frequency of the filter.
      cutoff_hz = 10
      # Use firwin with a Kaiser window to create a lowpass FIR filter.
      taps = firwin(N, cutoff_hz/nyq_rate, window=('kaiser', beta))
      # Use lfilter to filter x with the FIR filter.
      
      print taps
      delay = 0.5 * (N-1) / sample_rate
      print "Phase delay is "  , delay

      return lfilter(taps, 1.0, sig)
  


(pitch_kn,pitch_vel_kn,pitch_acc_kn,drift_kn,P) = kalman4(d)


    
subplot(4,1,1)
plot(pitch_kn, label="kalman pc")
plot(d.att_pitch, label="kalman drone")

plot([x-5 for x in integrate(d.att_gy)], label="gyro integrated")
plot([x-5 for x in integrate(d.att_filtered_gy)], label="filtered gyro integrated")
#plot(d.att_gy,label='gyro')
#plot(lowpass(d.att_gy),label='gyro lp')



#angle_from_accel_smoothed=moving_average_list([unwrap(rad2Deg(pitch_a(ax,az))) for (ax,az) in zip(d.att_ax, d.att_az)],8)

#plot(angle_from_accel_smoothed,label="accel, future")



grid()
legend()

#subplot(3,1,2)
#plot( [dr-pc for (dr,pc) in zip(d.att_pitch,pitch_kn)])



subplot(4,1,2)
plot(lowpass(d.att_gy),label="velocity, lowpass")
plot(d.att_pitch_vel_kalman,label="velocity, kalman")


#plot(d.att_gy,label="velocity, raw")


plot(pitch_vel_kn, label="velocity kalman pc")

velocity_from_gy_smothed=moving_average_list(d.att_gy,8)
plot(velocity_from_gy_smothed,label="velocity, future")

velocity_from_gy_smothed_lag=moving_average_lag_list(d.att_gy,4)
plot(velocity_from_gy_smothed_lag,label="velocity, moving_average")


#gyNoise=[a-b for a,b in zip(d.att_gy_kalman,velocity_from_gy_smothed)]
#plot(gyNoise,label="Noise in gy")
#m=mean(gyNoise)
#stddev= sum([ ((x-m)/180*pi)**2 for x in gyNoise])/(len(gyNoise)-1)
#print "variance of gy noise: {}".format(stddev)

grid()
legend()

#subplot(3,2,4)
#plot( [dr-pc for (dr,pc) in zip(d.att_gy_kalman,pitch_vel_kn)])

#grid()
#legend()

subplot(4,1,3)

plot(pitch_acc_kn, label="acceleration of pitch pc")
plot(pitch_vel_kn, label="velocity kalman pc")
plot(moving_average_list([x *200 for x in differentiate(velocity_from_gy_smothed) ],8), label="diff of vel future")
plot([ adj_pitch_to_pitch_accel(x) for x in d.adj_pitch ],label="adj_pitch")
plot(lowpass([x *200 for x in differentiate(d.att_filtered_gy) ]), label="filtered diff of vel gy filtered")


grid()
legend()

subplot(4,1,4)

fft= abs(np.fft.rfft(d.att_gx))**2
points=len(fft)-1
fft=fft[1:points+1]
freqs=arange(0,points,1.0)* ( 200.0/len(d.att_gx))
#plot(freqs,fft, label="fft gyro")

fft= abs(np.fft.rfft(lowpass(d.att_gx)))**2
points=len(fft)-1
fft=fft[1:points+1]
freqs=arange(0,points,1.0)* ( 200.0/len(d.att_filtered_gx))
plot(freqs,fft, label="fft filtered gyro")

#plot(drift_kn, label="drift pc")
#plot(d.att_gy_bias_kalman,label="drift drone")
#plot( [ x[0,0] for  x in P ],label="covariance w")
#plot( [ x[1,1] for  x in P ],label="covariance vel")
#plot( [ x[2,2] for  x in P ],label="covariance drift")



#plot([ adj_pitch_to_pitch_accel(x) for x in d.adj_pitch ],label="adj_pitch")
grid()
legend()


#subplot(3,2,6)
#plot( [dr-pc for (dr,pc) in zip(d.att_gy_bias_kalman,drift_kn)])


      
show()


