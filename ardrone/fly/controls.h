#ifndef _CONTROLS_H
#define _CONTROLS_H

struct setpoint_struct {
	float pitch; //radians  
	float roll; //radians     
	float yaw; //radians   
	float h; //meters
};


struct control_limits_struct {
	float pitch_roll_max; //radians     
	float h_max; //m
	float h_min; //m
	float throttle_hover; //hover throttle setting
	float throttle_min; //min throttle (while flying)
	float throttle_max; //max throttle (while flying)
};



enum FlyState {
	Landed=1,
	Launching=10,
	Flying=11,
	Landing=12,
	Error=20
} flyState;

const char *stateName(enum FlyState state);
void switchState(enum FlyState newState);

extern struct setpoint_struct setpoint;
extern struct control_limits_struct control_limits;
extern enum FlyState flyState;


#endif


