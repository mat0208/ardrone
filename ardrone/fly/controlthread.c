/*
 controlthread.c - AR.Drone control thread

 Copyright (C) 2011 Hugo Perquin - http://blog.perquin.com

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 MA 02110-1301 USA.
 */

#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <stdlib.h>  //exit()
#include <pthread.h>
#include <math.h>

#include "../util/type.h"
#include "../util/util.h"
#include "../motorboard/mot.h"
#include "../attitude/attitude.h"
#include "../udp/udp.h"
#include "pid.h"
#include "controlthread.h"

float adj_roll;
float adj_pitch;
float adj_yaw;
float adj_h;

pthread_t ctl_thread;

struct pid_struct pid_roll;
struct pid_struct pid_pitch;
struct pid_struct pid_yaw;
struct pid_struct pid_h;

float throttle;

#define MOTOR_TAKEOF_THROTTLE 0.55
/** ramp progress while launching*/
#define LAUNCHRAMP_LENGTH 800 // 200 ^= 1 second
int launchRamp;


struct att_struct att;

/** @todo split this structure in setpoints and limits */
struct setpoint_struct {
	float pitch; //radians  
	float roll; //radians     
	float yaw; //radians   
	float h; //meters
	float pitch_roll_max; //radians     
	float h_max; //m
	float h_min; //m
	float throttle_hover; //hover throttle setting
	float throttle_min; //min throttle (while flying)
	float throttle_max; //max throttle (while flying)
} setpoint;

struct setpoint_struct setpoint_landing={0,0,0,0.2, 0,0,0,0,0.5,0.85};


enum FlyState {
	Landed=1,
	Launching=10,
	Flying=11,
	Landing=12,
	Error=20
} flyState;

const char *stateName(enum FlyState state)
{
	switch(state) {
	  case Landed: return "Landed"; break;
	  case Launching: return "Launching"; break;
	  case Flying: return "Flying"; break;
	  case Landing: return "Landing"; break;
	  case Error: return "Error"; break;
	  default: return "Unknown"; break;
	}
}




void switchState(enum FlyState newState)
{
  printf("Switching state to %d: %s\n",newState, stateName(newState));
  flyState=newState;
  
  if(newState==Launching) launchRamp=0;
}


struct udp_struct udpNavLog;
int logcnt = 0;
void navLog_Send();
void *ctl_thread_main(void* data);

int ctl_Init(char *client_addr) {
	int rc;
 
	//defaults from AR.Drone app:  pitch,roll max=12deg; yawspeed max=100deg/sec; height limit=on; vertical speed max=700mm/sec; 
	setpoint.pitch_roll_max = DEG2RAD(12); //degrees     
	//setpoint.yawsp_max=DEG2RAD(100); //degrees/sec
	setpoint.h_max = 6.00;
	setpoint.h_min = 0.40;
	setpoint.throttle_hover = 0.66;
	setpoint.throttle_min = 0.50;
	setpoint.throttle_max = 0.85;

	//init pid pitch/roll 
	pid_Init(&pid_roll, 0.50, 0, 0, 0);
	pid_Init(&pid_pitch, 0.50, 0, 0, 0);
	pid_Init(&pid_yaw, 1.00, 0, 0, 0);
	pid_Init(&pid_h, 0.05, 0, 0, 0);

	throttle = 0.00;

	//Attitude Estimate
	rc = att_Init(&att);
	if (rc)
		return rc;

	//udp logger
	if (client_addr) {
		udpClient_Init(&udpNavLog, client_addr, 7778);
		navLog_Send();
		printf("udpClient_Init\n", rc);
	}

	//start motor thread
	rc = mot_Init();
	if (rc)
		return rc;

	//start ctl thread 
	rc = pthread_create(&ctl_thread, NULL, ctl_thread_main, NULL);
	if (rc) {
		printf("ctl_Init: Return code from pthread_create(mot_thread) is %d\n",
				rc);
		return 202;
	}
}

void calculateMotorSpeedsFlying(float motor[4], struct setpoint_struct setpoint)
{
	//flying, calc pid controller corrections
	adj_roll = pid_CalcD(&pid_roll, setpoint.roll - att.roll, att.dt,
			att.gx); //err positive = need to roll right
	adj_pitch = pid_CalcD(&pid_pitch, setpoint.pitch - att.pitch,
			att.dt, att.gy); //err positive = need to pitch down
	adj_yaw = pid_CalcD(&pid_yaw, setpoint.yaw - att.yaw, att.dt,
			att.gz); //err positive = need to increase yaw to the left
	adj_h = pid_CalcD(&pid_h, setpoint.h - att.h, att.dt, att.hv); //err positive = need to increase height

	throttle = setpoint.throttle_hover + adj_h;
	if (throttle < setpoint.throttle_min)
		throttle = setpoint.throttle_min;
	if (throttle > setpoint.throttle_max)
		throttle = setpoint.throttle_max;

	//convert pid adjustments to motor values
	motor[0] = throttle + adj_roll - adj_pitch + adj_yaw;
	motor[1] = throttle - adj_roll - adj_pitch - adj_yaw;
	motor[2] = throttle - adj_roll + adj_pitch + adj_yaw;
	motor[3] = throttle + adj_roll + adj_pitch - adj_yaw;

}

void *ctl_thread_main(void* data) {
	int cnt;
	int rc;
	switchState(Landed);

	while (1) {
		rc = att_GetSample(&att);
		if (!rc)
			break;
		if (rc != 1)
			printf("ctl_thread_main: att_GetSample return code=%d", rc);
	}

	while (1) {
		//get sample
		while (1) {
			rc = att_GetSample(&att); //non blocking call
			if (!rc)
				break; //got a sample
			if (rc != 1)
				printf("ctl_thread_main: att_GetSample return code=%d", rc);
		}

		float motor[4];
		
		switch(flyState) {
		  case Landed:
		    for(int i=0;i<4;i++) motor[i]=0;
		    if(setpoint.h>0) switchState(Launching);
		  break;
		  case Launching:
		    launchRamp++;
		    for(int i=0;i<4;i++) motor[i]=launchRamp*MOTOR_TAKEOF_THROTTLE/LAUNCHRAMP_LENGTH;
		    if (att.h > 0 || launchRamp > LAUNCHRAMP_LENGTH) {
		    	switchState(Flying);
		    }
		  break;
		  
		  case Flying:
		    calculateMotorSpeedsFlying(motor,setpoint);
		    if (setpoint.h < 0.1) switchState(Landing);
		  break;
		  
		  case Landing:
		    if (att.h > 0.2) {
		    	calculateMotorSpeedsFlying(motor,setpoint_landing);
		    } else {
		      switchState(Landed);
	            }
		  break;
		  
		  case Error:
		    for(int i=0;i<4;i++) motor[i]=0;
		    if(setpoint.h==0) switchState(Landed);
		  break;
		}
		 
		if ((cnt % 200) == 0) {
			printf("SET ROLL %5.2f PITCH %5.2f YAW %5.2f   H %5.2f\n",
					setpoint.roll, setpoint.pitch, setpoint.yaw, setpoint.h);
			printf("ATT ROLL %5.2f PITCH %5.2f YAW %5.2f   H %5.2f\n", att.roll,
					att.pitch, att.yaw, att.h);
			printf("ADJ ROLL %5.2f PITCH %5.2f YAW %5.2f THR %5.2f\n", adj_roll,
					adj_pitch, adj_yaw, throttle);
		}
	


		//send to motors
		mot_Run(motor[0], motor[1], motor[2], motor[3]);

		//blink leds    
		cnt++;
		if ((cnt % 200) == 0)
			mot_SetLeds(MOT_LEDGREEN, MOT_LEDGREEN, MOT_LEDGREEN, MOT_LEDGREEN);
		else if ((cnt % 200) == 100)
			mot_SetLeds(0, 0, 0, 0);

		navLog_Send();

		//yield to other threads
		pthread_yield();
	}
}

//logging
void navLog_Send() {
	char logbuf[1024];
	int logbuflen;

	float motval[4];
	mot_GetMot(motval);

	logcnt++;
	logbuflen = sprintf(logbuf,
			"%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f"
			//sequence+timestamp
			, logcnt, att.ts // navdata timestamp in sec
			//sensor data
			, att.ax // acceleration x-axis in [m/s^2] front facing up is positive         
			, att.ay // acceleration y-axis in [m/s^2] left facing up is positive                
			, att.az // acceleration z-axis in [m/s^2] top facing up is positive             
			, RAD2DEG(att.gx) // gyro value x-axis in [deg/sec] right turn, i.e. roll right is positive           
			, RAD2DEG(att.gy) // gyro value y-axis in [deg/sec] right turn, i.e. pitch down is positive                     
			, RAD2DEG(att.gz) // gyro value z-axis in [deg/sec] right turn, i.e. yaw left is positive 
			, att.hv // vertical speed [m/sec]
			//height
			, setpoint.h // setpoint height
			, att.h // actual height above ground in [m] 
			, throttle // throttle setting 0.00 - 1.00
			//pitch
			, RAD2DEG(setpoint.pitch) //setpoint pitch [deg]
			, RAD2DEG(att.pitch) //actual pitch   
			, adj_pitch //pitch motor adjustment 
			//roll
			, RAD2DEG(setpoint.roll) //setpoint roll [deg]
			, RAD2DEG(att.roll) //actual roll  
			, adj_roll //roll motor adjustment 
			//yaw
			, RAD2DEG(setpoint.yaw) //yaw pitch [deg]
			, RAD2DEG(att.yaw) //actual yaw  
			, adj_yaw //yaw motor adjustment
			);
	udpClient_Send(&udpNavLog, logbuf, logbuflen);
}

int ctl_FlatTrim() {
	return att_FlatTrim(&att);
}

void ctl_SetSetpoint(float pitch, float roll, float yaw, float h) {
	if (pitch > setpoint.pitch_roll_max)
		pitch = setpoint.pitch_roll_max;
	if (pitch < -setpoint.pitch_roll_max)
		pitch = -setpoint.pitch_roll_max;
	setpoint.pitch = pitch;
	if (roll > setpoint.pitch_roll_max)
		roll = setpoint.pitch_roll_max;
	if (roll < -setpoint.pitch_roll_max)
		roll = -setpoint.pitch_roll_max;
	setpoint.roll = roll;
	//if(yaw > setpoint.yawsp_max) yaw = setpoint.yawsp_max;
	//if(yaw < -setpoint.yawsp_max) yaw = -setpoint.yawsp_max;
	setpoint.yaw = yaw;
	if (h > setpoint.h_max)
		h = setpoint.h_max;
	if (h <= 0)
		h = 0;
	if (h > 0 && h < setpoint.h_min)
		h = setpoint.h_min;
	setpoint.h = h;
}

void ctl_SetSetpointDiff(float pitch, float roll, float yaw, float h) {
	ctl_SetSetpoint(pitch + setpoint.pitch, setpoint.pitch + pitch,
			yaw + setpoint.yaw, h + setpoint.h);
}

void ctl_Close() {
	mot_Close();
	att_Close();
}

void ctl_SetGas(float gas1) {
	throttle += gas1;
}

