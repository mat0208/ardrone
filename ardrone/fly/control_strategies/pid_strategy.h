#ifndef _PID_STRATEGY
#define _PID_STRATEGY

#include "../controls.h"
#include "../../attitude/attitude.h"


void pidStrategy_init();
void pidStrategy_calculateMotorSpeeds(enum FlyState flyState, struct att_struct att, struct setpoint_struct setpoint, float motorOut[4]);
/** puts interesting logging information into buffer, returning the number of bytes written, format is csv */
unsigned int pidStrategy_getStateForLog(char *buf,unsigned int maxLen);


#endif