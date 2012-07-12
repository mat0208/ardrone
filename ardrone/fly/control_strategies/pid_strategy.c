#include "pid.h"
#include "../controls.h"
#include "pid_strategy.h"
#include <stdio.h>

struct pid_struct pid_roll;
struct pid_struct pid_pitch;
struct pid_struct pid_yaw;
struct pid_struct pid_h;

float adj_roll;
float adj_pitch;
float adj_yaw;
float adj_h;

float throttle;

#define MOTOR_TAKEOF_THROTTLE 0.55
/** ramp progress while launching*/
#define LAUNCHRAMP_LENGTH 800 // 200 ^= 1 second
int launchRamp;

struct setpoint_struct setpoint_landing={0,0,0,0.2, 0,0,0,0,0.5,0.85};


void pidStrategy_init()
{
	//init pid pitch/roll 
	pid_Init(&pid_roll,  0.015,  0.000, 0.01, 0.1);
	pid_Init(&pid_pitch, 0.015,  0.000, 0.01, 0.1);

	pid_Init(&pid_yaw, 1.00, 0, 0, 0);
	pid_Init(&pid_h, 0.05, 0, 0, 0);
	throttle = 0.00;


}

void pidStrategy_calculateMotorSpeedsFlying(struct att_struct att, struct setpoint_struct setpoint, float motorOut[4])
{
	//flying, calc pid controller corrections
	adj_roll = pid_CalcD(&pid_roll, setpoint.roll - att.roll, att.dt,
			att.gx); //err positive = need to roll right
	adj_pitch = pid_CalcD(&pid_pitch, setpoint.pitch - att.pitch,
			att.dt, -att.gy); //err positive = need to pitch down
	adj_yaw = pid_CalcD(&pid_yaw, setpoint.yaw - att.yaw, att.dt,
			-att.gz); //err positive = need to increase yaw to the left
	adj_h = pid_CalcD(&pid_h, setpoint.h - att.h, att.dt, att.hv); //err positive = need to increase height

	throttle = setpoint.throttle_hover + adj_h;
	if (throttle < setpoint.throttle_min)
		throttle = setpoint.throttle_min;
	if (throttle > setpoint.throttle_max)
		throttle = setpoint.throttle_max;

	//convert pid adjustments to motor values
	motorOut[0] = throttle + adj_roll - adj_pitch + adj_yaw;
	motorOut[1] = throttle - adj_roll - adj_pitch - adj_yaw;
	motorOut[2] = throttle - adj_roll + adj_pitch + adj_yaw;
	motorOut[3] = throttle + adj_roll + adj_pitch - adj_yaw;
}


void pidStrategy_calculateMotorSpeeds(enum FlyState flyState, struct att_struct att, struct setpoint_struct setpoint, float motorOut[4])
{
		switch(flyState) {
		  case Landed:
		    for(int i=0;i<4;i++) motorOut[i]=0;
		    if(setpoint.h>0) switchState(Launching);
		  break;
		  case Launching:
		    launchRamp++;
		    for(int i=0;i<4;i++) motorOut[i]=launchRamp*MOTOR_TAKEOF_THROTTLE/LAUNCHRAMP_LENGTH;
		    if (att.h > 0 || launchRamp > LAUNCHRAMP_LENGTH) {
		    	switchState(Flying);
		    }
		    if (setpoint.h < 0.1) switchState(Landing);
		  break;
		  
		  case Flying:
		    pidStrategy_calculateMotorSpeedsFlying(att,setpoint,motorOut);
		    if (setpoint.h < 0.1) switchState(Landing);
		  break;
		  
		  case Landing:
		    if (att.h > 0.32) {
		    	pidStrategy_calculateMotorSpeedsFlying(att,setpoint_landing,motorOut);
		    } else {
		      switchState(Landed);
	            }
		  break;
		  
		  case Error:
		    for(int i=0;i<4;i++) motorOut[i]=0;
		    if(setpoint.h==0) switchState(Landed);
		    launchRamp=0;
		  break;
		}
}

unsigned int pidStrategy_getStateForLog(char *buf,unsigned int maxLen)
{
  int len;
  len= snprintf(buf,maxLen,
        "%f,%f,%f,%f,"
        ,adj_roll
        ,adj_pitch
        ,adj_yaw
        ,adj_h
      );
  return len;      

}



