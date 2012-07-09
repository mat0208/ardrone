#include <iostream>
#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <signal.h>
#include <boost/version.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include "Drone.hpp"
#include "Joystick.hpp"

#if (BOOST_VERSION < 104700)
#error "Boost 1.47 or greater required (for boost::asio::signal_set support: sudo apt-get install libboost1.48-dev)"
#endif

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
namespace bpo = boost::program_options;

inline double DEG2RAD(double x)
{
  return x * 3.1415926 / 180;
}

inline double RAD2DEG(double x)
{
  return x / 3.1415926 * 180;
}


static void set_input_flags(bool flag)
{
  struct termios t;
  tcgetattr(fileno(stdin), &t);
  if (flag)
    t.c_lflag |= ICANON|ECHO;
  else
    t.c_lflag &= ~(ICANON|ECHO);
  tcsetattr(fileno(stdin), TCSANOW, &t);
}

static void show_control_tips()
{
  cout	<< "Controls: 	                                        " << endl
        << "Space = Land/Init, Enter = Takeoff        		" << endl
        << "  W  =       Forward                                " << endl
        << " SAD =  Left/Backward/Right                       	" << endl
        << "  I  =              Up                              " << endl
        << " JKL =  Rotate Left/Down/Rotate Right             	" << endl
        << " Q = Land and Quit					" << endl
        << " N = Neutral (stop WSAD movement)		        " << endl
        << endl;
}

const double takeoff_height = 0.75;

void runKeyboardControlled(Drone& drone)
{
  class scoped_input_flags {
    public:
      scoped_input_flags()
      {
        set_input_flags(false);
      }
      ~scoped_input_flags()
      {
        set_input_flags(true);
      }
  };
  scoped_input_flags flags; // make sure echo is on after program ends
  const double pitch_roll_step = 1.0;
  const double yaw_step = 5.0;
  const double height_step = 0.05;
  show_control_tips();
  bool run = true;
  while(run) {
    char c;
    if (cin.read(&c, 1)) {
      cout << "                                                   \r";
      c = tolower(c);
      switch(c) {
        case 'n':
          drone.Pitch(0.0);
          drone.Roll(0.0);
          drone.SendCmd();
          break;
        case 'w':
          drone.Pitch(drone.Pitch() + DEG2RAD(pitch_roll_step));
          drone.SendCmd();
          break;
        case 's':
          drone.Pitch(drone.Pitch() - DEG2RAD(pitch_roll_step));
          drone.SendCmd();
          break;
        case 'a':
          drone.Roll(drone.Roll() - DEG2RAD(pitch_roll_step));
          drone.SendCmd();
          break;
        case 'd':
          drone.Roll(drone.Roll() + DEG2RAD(pitch_roll_step));
          drone.SendCmd();
          break;
        case 'k':
          drone.H(drone.H() - height_step);
          drone.SendCmd();
          break;
        case 'i':
          drone.H(drone.H() + height_step);
          drone.SendCmd();
          break;
        case 'j':
          drone.Yaw(drone.Yaw() + DEG2RAD(yaw_step));
          drone.SendCmd();
          break;
        case 'l':
          drone.Yaw(drone.Yaw() - DEG2RAD(yaw_step));
          drone.SendCmd();
        case 'q':
          run = false; //fallthrough
        case ' ':
          drone.Land();
          break;
        case '\n':
          drone.TakeOff(takeoff_height);
          break;
        default:
          cout << "Unknown key: '"<< c << "'                \r";
          break;
      }
    }
  }
}

class JoystickDroneHandler {
  enum AxisIndex {
    AxisRoll,
    AxisPitch,
    AxisYaw,
    AxisHeight,
  };

  enum ButtonIndex {
    ButtonTakeOff,
    ButtonLand,
  };
    
  Drone& drone_;
  Joystick& joystick_;
  static const unsigned joystick_dead_zone_percent = 20;
public:
  JoystickDroneHandler(Drone& drone, Joystick& joystick) : drone_(drone), joystick_(joystick)
  {
    joystick.setButtonHandler(boost::bind(&JoystickDroneHandler::handleButton, this, _1, _2));
    joystick.setAxisHandler(boost::bind(&JoystickDroneHandler::handleAxis, this, _1, _2));
  }

  void handleButton(uint8_t button, bool down)
  {
    if (down)
    switch(button) {
      case ButtonTakeOff:
        cout << "TakeOff " << down << endl;
        drone_.TakeOff(takeoff_height);
        break;
      case ButtonLand:
        drone_.Land();
        cout << "Land " << down << endl;
      break;
    }
  }

  void handleAxis(uint8_t axis, int16_t value)
  {
    switch(axis) {
      case AxisRoll:
        cout << "Roll " << value << endl;
        Roll(value);
        break;
      case AxisPitch:
        cout << "Pitch " << (value*(-1)) << endl;
        Pitch(value*(-1));
        break;
      case AxisYaw:
        cout << "Yaw " << value << endl;
        Yaw(value);
        break;
      case AxisHeight:
        cout << "Height " << (value*(-1)) << endl;
        Height(value*(-1));
        break;
    }
  }
  
  void Roll(int16_t value) {
    //drone_.Roll();
  }
  
  void Pitch(int16_t value) {
    //drone_.Pitch();
  }
  
  void Yaw(int16_t value) {
    //drone_.Yaw();
  }
  
  void Height(int16_t value) {
   //drone_.H();    
  }
  
  void stop() {
    cerr << "landing drone and stopping joy :-(" << endl;
    drone_.Land();
    joystick_.stop();
  }
};

void runJoystickControlled(Drone& drone, const string& joystickDevice)
{
  Joystick js(drone.io_service(), joystickDevice);
  boost::asio::signal_set signals(drone.io_service());
  signals.add(SIGINT);
  signals.add(SIGTERM);
#ifdef SIGQUIT
  signals.add(SIGQUIT);
#endif
  JoystickDroneHandler joyHandler(drone, js);
  js.start();
  signals.async_wait(boost::bind(&JoystickDroneHandler::stop, &joyHandler));
  drone.io_service().run();
}

void runConsole(const bpo::variables_map& vm)
{
  boost::asio::io_service srv;  Drone drone(vm["address"].as<string>(), srv);
  if (vm.count("joystick")) {
    runJoystickControlled(drone, vm["joystick"].as<string>());
  } else {
    runKeyboardControlled(drone);
  }
}

void printUsage(const string& program, const bpo::options_description& options)
{
  cerr << program << " [options] " << endl << options << endl;
}

int main(int argc, char **argv) {
  try {
    bpo::options_description options("Options");
    options.add_options()
      ("help,h", "print this help message")
      ("address", bpo::value<string>()->default_value("192.168.1.1"), "address of the drone")
      ("joystick", bpo::value<string>(), "use joystick with arg as device name")
    ;
    bpo::options_description command_line_options;
    command_line_options.add(options);
    bpo::variables_map vm;
    bpo::store(bpo::command_line_parser(argc, argv).options(command_line_options).run(), vm);
    bpo::notify(vm);
    if (vm.count("help")) {
      printUsage(argv[0], options);
      return 0;
    }
    runConsole(vm);
  }
  catch(const std::exception& e) {
    cerr << "exception: " << e.what() << endl;
    return 1;
  }
  return 0;
}
