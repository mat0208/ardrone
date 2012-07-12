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
#include "control_strategies/pid_strategy.h"
#include "controlthread.h"
#include "controls.h"

float adj_roll;
float adj_pitch;
float adj_yaw;
float adj_h;

pthread_t ctl_thread;


float motor[4];

#define MAX_LOGBUFSIZE 1024

struct att_struct att;

/** @todo split this structure in setpoints and limits */






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


	//Attitude Estimate
	rc = att_Init(&att);
	if (rc)
		return rc;

	//udp logger
	if (client_addr) {
		udpClient_Init(&udpNavLog, client_addr, 7778);
		navLog_Send();
		printf("udpClient_Init %d\n", rc);
	}

	//start motor thread
	rc = mot_Init();
	if (rc)
		return rc;
		
	pidStrategy_init();

	//start ctl thread 
	rc = pthread_create(&ctl_thread, NULL, ctl_thread_main, NULL);
	if (rc) {
		printf("ctl_Init: Return code from pthread_create(mot_thread) is %d\n",
				rc);
		return 202;
	}
	return 0;
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

                pidStrategy_calculateMotorSpeeds(flyState, att, setpoint, motor);		
		 
		if ((cnt % 200) == 0) {
			printf("SET ROLL %5.2f PITCH %5.2f YAW %5.2f   H %5.2f\n",
					setpoint.roll, setpoint.pitch, setpoint.yaw, setpoint.h);
			printf("ATT ROLL %5.2f PITCH %5.2f YAW %5.2f   H %5.2f\n", att.roll,
					att.pitch, att.yaw, att.h);
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
	char logbuf[MAX_LOGBUFSIZE];
	int logbuflen;

	float motval[4];
	mot_GetMot(motval);

	logcnt++;
	logbuflen = snprintf(logbuf,MAX_LOGBUFSIZE,
			"%d,%f,%d,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,"
			//sequence+timestamp
			, logcnt, att.ts // navdata timestamp in sec
			, flyState
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
			, (motval[0]+motval[1]+motval[2]+motval[3])/4 // throttle setting 0.00 - 1.00
			//pitch
			, RAD2DEG(setpoint.pitch) //setpoint pitch [deg]
			, RAD2DEG(att.pitch) //actual pitch   
			//roll
			, RAD2DEG(setpoint.roll) //setpoint roll [deg]
			, RAD2DEG(att.roll) //actual roll  
			//yaw
			, RAD2DEG(setpoint.yaw) //yaw pitch [deg]
			, RAD2DEG(att.yaw) //actual yaw  
			, motval[0]
			, motval[1]
			, motval[2]
			, motval[3]
			);
			
	logbuflen+=pidStrategy_getStateForLog(logbuf+logbuflen,MAX_LOGBUFSIZE-logbuflen);
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


