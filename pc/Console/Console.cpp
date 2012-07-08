#include <iostream>
#include <cstdio>
#include "Drone.hpp"
#include <termios.h>
#include <unistd.h>

using std::cin;

inline double DEG2RAD(double x)
{
  return x * 3.1415926 / 180;
}
inline double RAD2DEG(double x)
{
  return x / 3.1415926 * 180;
}


int
set_canon(int flag)
{
        struct termios t;

        tcgetattr( fileno(stdin), &t);
        if( flag)
                t.c_lflag |= ICANON;
        else
                t.c_lflag &= ~ICANON;
        tcsetattr( fileno(stdin), TCSANOW, &t); 

        return( 1);
}


int main(int argc, char **argv) {
  set_canon(0);
  Drone drone(argc > 1 ? argv[1] : "192.168.1.1");
  const double pitch_roll_step = 1.0;
  const double yaw_step = 5.0;
  bool run = true;
  while(run) {
    char c;
    if (cin.read(&c, 1)) {
      c = tolower(c);
      switch(c) {	
        //K Neutral
        case 'k': 
          drone.Pitch(0.0);
          drone.Roll(0.0);
          drone.SendCmd();
          break;
        //I Fly forward
        case 'i':
            drone.Pitch(drone.Pitch() + DEG2RAD(pitch_roll_step));
            drone.SendCmd();
            break;
        //, Fly backward
        case ',':
            drone.Pitch(drone.Pitch()-DEG2RAD(pitch_roll_step));
            drone.SendCmd();
            break;
        //J Fly leftward
        case 'j':
            drone.Roll(drone.Roll() -DEG2RAD(pitch_roll_step));
            drone.SendCmd();
            break;
        //L Fly rightward
        case 'l':
            drone.Roll(drone.Roll() + DEG2RAD(pitch_roll_step));
            drone.SendCmd();
            break;
        //S Throttle down
        case 's':
            drone.H(drone.H() - 5);
            drone.SendCmd();
            break;
        //W Throttle up
        case 'w':
            drone.H(drone.H() + 5);
            drone.SendCmd();
            break;
        //A Rotate anticlockwise
        case 'a':
            drone.Yaw(drone.Yaw() + DEG2RAD(yaw_step));
            drone.SendCmd();
            break;
        //D Rotate clockwise
        case 'd':
            drone.Yaw(drone.Yaw()- DEG2RAD(yaw_step));
            drone.SendCmd();
         //Q Quit
        case 'q': 
          run = false;
        //Space Land
        case ' ':
            drone.Pitch(0.0);
            drone.Roll(0.0);
            drone.H(0.0);
            drone.SendCmd();
            break;
        //Return takeoff
        case '\n':
            drone.Pitch(0.0);
            drone.Roll(0.0);
            drone.H(75.0);
            drone.SendCmd();
            break;

      }
    }
  }
  return 0;
}