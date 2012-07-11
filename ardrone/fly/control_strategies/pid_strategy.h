#ifndef _PID_STRATEGY
#define _PID_STRATEGY

#include "../controls.h"
#include "../../attitude/attitude.h"


void pidStrategy_init();
void pidStrategy_calculateMotorSpeeds(enum FlyState flyState, struct att_struct att, struct setpoint_struct setpoint, float motorOut[4]);



#endif