import os, sys
lib_path = os.path.abspath(os.path.join('..','dronelib'))
sys.path.append(lib_path)
lib_path = os.path.abspath(os.path.join('..','simlib'))
sys.path.append(lib_path)
from dronelib import *
from memoize import *
from simlib import *
import time
from scipy.signal import kaiserord, lfilter, firwin, freqz
import scipy as S



def angleDiff(an1,an2):
    difference = an1 - an2
    while difference < -pi:
        difference += 2*pi
    while difference > pi:
        difference -= 2*pi
    return difference
    

d=readCsv('simtest.csv')
rollRecV =S.interpolate.interp1d(d.att_ts,d.att_roll )
pitchRecV=S.interpolate.interp1d(d.att_ts,d.att_pitch)
yawRecV  =S.interpolate.interp1d(d.att_ts,d.att_yaw  )


def squareError(x,t,d):
    rollSim=x[0]
    pitchSim=x[2]
    yawSim=x[4]

    rollRec=deg2Rad(rollRecV(t))
    pitchRec=deg2Rad(pitchRecV(t))
    yawRec=deg2Rad(yawRecV(t))


    rollD=angleDiff(rollSim,rollRec)
    pitchD=angleDiff(pitchSim, pitchRec)
    yawD=angleDiff(yawSim,yawRec)

    return rollD**2+pitchD**2+yawD**2




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
      #print "N is " , N
      # The cutoff frequency of the filter.
      cutoff_hz = 15
      # Use firwin with a Kaiser window to create a lowpass FIR filter.
      taps = firwin(N, cutoff_hz/nyq_rate, window=('kaiser', beta))
      # Use lfilter to filter x with the FIR filter.
      
      #print taps
      delay = 0.5 * (N-1) / sample_rate
      print "Phase delay is "  , delay

      return lfilter(taps, 1.0, sig)

d.motval_0=lowpass(d.motval_0)
d.motval_1=lowpass(d.motval_1)
d.motval_2=lowpass(d.motval_2)
d.motval_3=lowpass(d.motval_3)


mv0 = S.interpolate.interp1d(d.att_ts,d.motval_0, kind='linear')
mv1 = S.interpolate.interp1d(d.att_ts,d.motval_1, kind='linear')
mv2 = S.interpolate.interp1d(d.att_ts,d.motval_2, kind='linear')
mv3 = S.interpolate.interp1d(d.att_ts,d.motval_3, kind='linear')

@memoize
def recordedController(t):
    m1=mv0(t)
    m2=mv1(t)
    m3=mv2(t)
    m4=mv3(t)

    return [m1,m2,m3,m4]
    



#noiseTime=N.arange(0,tEnd,0.1)
#rollNoise=normal(0,1,len(noiseTime))
#pitchNoise=normal(0,1,len(noiseTime)) 

def rollNoiseFunc(tNoise):
    rollN=N.interp(tNoise,noiseTime,rollNoise)
    pitchN=N.interp(tNoise,noiseTime,pitchNoise)

    
    return np.array([
        rollN,
        0,
        pitchN,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0,
        0
        ])


tEnd=min(max(d.att_ts),40.0)
x = N.arange(0,tEnd, 0.5)
t = x

def simError(y):
    totalError=0
    for num,ti in enumerate(t):
        stepError=squareError(y[num,:],ti,d)
        #print "Error at step ",num," t: ",ti,": ",stepError
        totalError+=stepError
    return totalError
    
rController=lambda x,t: recordedController(t)

def simOnce(params):
    

    y0=[0]*12
    y0[0]=deg2rad(d.att_roll[0])
    y0[2]=deg2rad(d.att_pitch[0])
    y0[4]=deg2rad(d.att_yaw[0])

    #oneController=lambda x,t: nullController(x,thrust=1.0)
    customFunc=lambda t,x: droneFunc(x,t,simParams=params,controller=rController)

 
    start_time = time.time()

    ode = S.integrate.ode(customFunc)
    #ode15s.set_integrator('vode', method='bdf', order=15, nsteps=3000)
    ode.set_initial_value(y0, 0.0)

    y=N.array([y0])
    i=0
    for tstep in t[1:]:
 #     i=i+1
  #      if i % 10 == 0 :
 #           print tstep
  
        yp=ode.integrate(tstep)
        y=N.append(y,N.array([yp]),axis=0)
        yp[0]=deg2Rad(rollRecV(tstep))
        yp[2]=deg2Rad(pitchRecV(tstep))
        yp[4]=deg2Rad(yawRecV(tstep))
   #     ode.set_initial_value(yp, tstep)

    #y = odeint(customFunc, y0, t)
    elapsed_time = time.time() - start_time
    print 'int end after ',elapsed_time
    return y


def minimizeProblem(x):
    p=SimParams()
    print "Sim with ",x
    [p.I_xx,p.I_yy,p.I_zz,p.J_r,p.d,p.b ]=x
    
    result=simOnce(p)
    error=simError(result)
    print "Error: ",error
    return error

x0= [0.0075, 0.0075, 0.0260, 0.00006, 0.00000050, 0.0000113]
    
#xbest=S.optimize.fmin(minimizeProblem, x0)

p=SimParams()
xbest=x0
print "Try with ",xbest
[p.I_xx,p.I_yy,p.I_zz,p.J_r,p.d,p.b ]=xbest
    
y=simOnce(p)
print "Sim error: ",simError(y)

subplot(3,1,1)
plot(t,y[:,0],label="roll sim")
plot(d.att_ts,[deg2rad(x) for x in d.att_roll],label="roll rec")

legend()
grid()

subplot(3,1,2)
plot(t,y[:,2],label="pitch sim")
plot(d.att_ts,[deg2rad(x) for x in d.att_pitch],label="pitch rec")

legend()
grid()


subplot(3,1,3)
plot(t,y[:,4],label="yaw sim")
plot(d.att_ts,[deg2rad(x) for x in d.att_yaw],label="yaw rec")

legend()
grid()


show()


