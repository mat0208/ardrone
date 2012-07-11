#include "controls.h"
#include <stdio.h>

struct setpoint_struct setpoint;
enum FlyState flyState;

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
}
