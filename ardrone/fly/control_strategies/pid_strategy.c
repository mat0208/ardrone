#include "pid.h"
#include "../controls.h"
#include "../../util/util.h"
#include "pid_strategy.h"
#include <stdio.h>

struct pid_struct pid_roll;
struct pid_struct pid_pitch;
struct pid_struct pid_yaw;


struct pid_struct pid_hor_vel_x;
struct pid_struct pid_hor_vel_y;

struct pid_struct pid_hor_pos_x;
struct pid_struct pid_hor_pos_y;

struct pid_struct pid_h;

double targetXVel=0;
double targetYVel=0;

double targetRoll=0;
double targetPitch=0;

float adj_roll;
float adj_pitch;
float adj_yaw;
float adj_h;

float throttle;


// minimum throttle for launching ramp 
#define MOTOR_INIT_THROTTLE 0.1
// ramp end throttle
#define MOTOR_TAKEOF_THROTTLE 0.55
/** ramp progress while launching*/
#define LAUNCHRAMP_LENGTH 600 // 200 ^= 1 second
int launchRamp=0;


// when landing, start ramp below this height over ground
#define LANDING_HEIGHT_START_RAMP 0.35

// throttle for landing part below min flight height (now height info)
#define MOTOR_LAND_MAX_THROTTLE  0.50
// ramp end throttle
#define MOTOR_LAND_MIN_THROTTLE 0.1
/** ramp progress while landing*/
#define LANDRAMP_LENGTH 300 // 200 ^= 1 second
int landRamp=0;


double xPos;
double yPos;

struct setpoint_struct setpoint_landing={0,0,0,0.2};

void pid_strategy_init()
{
	//init pid pitch/roll 
	pid_Init(&pid_roll,  0.5,  0.000, 0.00, 0.1);
	pid_Init(&pid_pitch, 0.5,  0.000, 0.00, 0.1);

	pid_Init(&pid_yaw, 0.1, 0, 0, 0);
	
	/** @todo these need an i-part */
	pid_Init(&pid_hor_vel_x, 0.2, 0.01, 0, 0.1);
	pid_Init(&pid_hor_vel_y, 0.2, 0.01, 0, 0.1);


	/** @todo these need an i-part */
	pid_Init(&pid_hor_pos_x, 5.0, 0.0, 0, 1);
	pid_Init(&pid_hor_pos_y, 5.0, 0.0, 0, 1);

	
	pid_Init(&pid_h, 0.03, 0.01, 0, 0.1);
	
	
	
	throttle = 0.00;


}

void pidStrategy_calculateMotorSpeedsFlying(struct horizontal_velocities_struct *hv, struct att_struct *att, struct setpoint_struct *setpoint, struct control_limits_struct *control_limits, float motorOut[4])
{
        /* overwrite setpoints for now */
        targetRoll=setpoint->roll;
        targetPitch=setpoint->pitch;
       
        

        if(1) {
          targetXVel=pid_Calc(&pid_hor_pos_x, 0-xPos, att->dt);
          targetYVel=pid_Calc(&pid_hor_pos_y, 0-yPos, att->dt);
          
        
          targetPitch=pid_Calc(&pid_hor_vel_x, targetXVel-hv->xv, att->dt);
          targetRoll =pid_Calc(&pid_hor_vel_y, targetYVel-hv->yv, att->dt);
        }   



	//flying, calc pid controller corrections
	adj_roll  = pid_CalcD(&pid_roll,  targetRoll  - att->roll , att->dt, att->gx); //err positive = need to roll right
	adj_pitch = pid_CalcD(&pid_pitch, targetPitch - att->pitch, att->dt, att->gy); //err positive = need to pitch down
	adj_yaw   = pid_CalcD(&pid_yaw,   setpoint->yaw   - att->yaw,   att->dt, att->gz); //err positive = need to increase yaw to the left
	adj_h     = pid_CalcD(&pid_h,     setpoint->h     - att->h,     att->dt, att->hv); //err positive = need to increase height

	throttle = control_limits->throttle_hover + adj_h;
	if (throttle < control_limits->throttle_min)
		throttle = control_limits->throttle_min;
	if (throttle > control_limits->throttle_max)
		throttle = control_limits->throttle_max;

	//convert pid adjustments to motor values
	motorOut[0] = throttle + adj_roll - adj_pitch + adj_yaw;
	motorOut[1] = throttle - adj_roll - adj_pitch - adj_yaw;
	motorOut[2] = throttle - adj_roll + adj_pitch + adj_yaw;
	motorOut[3] = throttle + adj_roll + adj_pitch - adj_yaw;
}


void pid_strategy_calculateMotorSpeeds(struct drone_state_struct *cs, float motorOut[4])
{
	struct setpoint_struct * setpoint=&cs->setpoint;
	switch(cs->flyState) {
	  case Landed:
		for(int i=0;i<4;i++) motorOut[i]=0;
		if(setpoint->h>0) switchState(cs,Launching);
		launchRamp=0;
		landRamp=0;
	  break;
	  case Launching:
		launchRamp++;
		for(int i=0;i<4;i++) motorOut[i]=launchRamp*(MOTOR_TAKEOF_THROTTLE-MOTOR_INIT_THROTTLE)/LAUNCHRAMP_LENGTH+MOTOR_INIT_THROTTLE;

		if (cs->att.h > 0 || launchRamp > LAUNCHRAMP_LENGTH) {
    		        xPos=0;
                        yPos=0;
			launchRamp=0;
			switchState(cs,Flying);
		}
		if (setpoint->h < 0.1) switchState(cs,Landing);
	  break;

	  case Flying:
	        xPos+=cs->hor_velocities.xv * cs->att.dt;
                yPos+=cs->hor_velocities.yv * cs->att.dt;
	                                    
		pidStrategy_calculateMotorSpeedsFlying(&cs->hor_velocities, &cs->att,setpoint,&cs->control_limits,motorOut);
		if (setpoint->h < 0.1) switchState(cs,Landing);
	  break;

	  case Landing:
	        xPos=0;
	        yPos=0;
		if (cs->att.h > LANDING_HEIGHT_START_RAMP) {
			pidStrategy_calculateMotorSpeedsFlying(&cs->hor_velocities, &cs->att,setpoint,&cs->control_limits,motorOut);
		} else if (landRamp< LANDRAMP_LENGTH) {
		  landRamp++;
                  for(int i=0;i<4;i++) motorOut[i]=MOTOR_LAND_MAX_THROTTLE- landRamp*(MOTOR_LAND_MAX_THROTTLE-MOTOR_LAND_MIN_THROTTLE)/LANDRAMP_LENGTH;
	        } else {	                 
	          landRamp=0; 
		  switchState(cs,Landed);
                }
	  break;

	  case Error:
		for(int i=0;i<4;i++) motorOut[i]=0;
		if(setpoint->h==0) switchState(cs,Landed);
		launchRamp=0;
	  break;
	}
}

unsigned int pid_strategy_getLogHeadings(char *buf, unsigned int maxLen)
{
  int len;
  len= snprintf(buf,maxLen, 
        ","
        "xPos,"
        "yPos,"
        "targetXVel,"
        "targetYVel,"
        "targetPitch [deg],"
        "targetRoll [deg],"
        "adj_pitch,"
        "adj_roll,"
        "adj_yaw,"
        "adj_h"
      );
  return len;
}

unsigned int pid_strategy_getLogText(char *buf,unsigned int maxLen)
{
  int len;
  len= snprintf(buf,maxLen,
        ",%f,%f,%f,%f,%f,%f,%f,%f,%f,%f"
        ,xPos
        ,yPos
        ,targetXVel
        ,targetYVel
        ,RAD2DEG(targetPitch)
        ,RAD2DEG(targetRoll)
        ,adj_pitch
        ,adj_roll
        ,adj_yaw
        ,adj_h
      );
  return len;      

}



