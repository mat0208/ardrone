#include <iostream>
#include <cstdio>
#include <signal.h>
#include <boost/version.hpp>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include <boost/date_time/posix_time/ptime.hpp>
#include <boost/date_time/posix_time/time_formatters_limited.hpp>
#include <memory>
#include <sstream>
#include "Drone.hpp"
#include "Joystick.hpp"
#include "Keyboard.hpp"
#include "UdpLogger.hpp"

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

const double takeoff_height = 0.5;

class KeybaordDroneHandler {
  Drone& drone_;
  Keyboard& keyboard_;
public:
  KeybaordDroneHandler(Drone& drone, Keyboard& keyboard) : drone_(drone), keyboard_(keyboard) 
  {
    keyboard_.setKeyHandler(boost::bind(&KeybaordDroneHandler::handleKey, this, _1));
  }

  ~KeybaordDroneHandler() {
  }

  void handleKey(char key) {
    const double pitch_roll_step = 1.0;
    const double yaw_step = 5.0;
    const double height_step = 0.05;
    switch(key) {
      case 'n':
        drone_.Pitch(0.0);
        drone_.Roll(0.0);
        drone_.SendCmd();
        break;
      case 'w':
        drone_.Pitch(drone_.Pitch() + DEG2RAD(pitch_roll_step));
        drone_.SendCmd();
        break;
      case 's':
        drone_.Pitch(drone_.Pitch() - DEG2RAD(pitch_roll_step));
        drone_.SendCmd();
        break;
      case 'a':
        drone_.Roll(drone_.Roll() - DEG2RAD(pitch_roll_step));
        drone_.SendCmd();
        break;
      case 'd':
        drone_.Roll(drone_.Roll() + DEG2RAD(pitch_roll_step));
        drone_.SendCmd();
        break;
      case 'k':
        drone_.H(drone_.H() - height_step);
        drone_.SendCmd();
        break;
      case 'i':
        drone_.H(drone_.H() + height_step);
        drone_.SendCmd();
        break;
      case 'j':
        drone_.Yaw(drone_.Yaw() + DEG2RAD(yaw_step));
        drone_.SendCmd();
        break;
      case 'l':
        drone_.Yaw(drone_.Yaw() - DEG2RAD(yaw_step));
        drone_.SendCmd();
      case 'q':
        stop();
        break;
      case ' ':
        drone_.Land();
        break;
      case '\n':
        drone_.TakeOff(takeoff_height);
        break;
      case '?':
        show_control_tips();
        break;
      default:
        cerr << "Unknown key: '"<< key << "'                \r";
        break;
    }
  }

  void stop() 
  {
    drone_.Land();
    keyboard_.stop();
    drone_.io_service().stop();
  }
};

void runKeyboardControlled(Drone& drone)
{
  show_control_tips();
  Keyboard keyboard(drone.io_service());
  boost::asio::signal_set signals(drone.io_service());
  signals.add(SIGINT);
  signals.add(SIGTERM);
#ifdef SIGQUIT
  signals.add(SIGQUIT);
#endif
  KeybaordDroneHandler handler(drone, keyboard);
  keyboard.start();
  signals.async_wait(boost::bind(&KeybaordDroneHandler::stop, &handler));
  drone.io_service().run();
}

class JoystickDroneHandler {
  enum AxisIndex {
    AxisRoll,
    AxisPitch,
    AxisYaw,
    AxisHeigth,
  };

  enum ButtonIndex {
    ButtonTakeOff,
    ButtonLand,
  };
    
  Drone& drone_;
  Joystick& joystick_;
  boost::asio::deadline_timer deadline_;
  float heigthAdj_;
  float yawAdj_;
  static const unsigned joystick_dead_zone_percent =20;
  static const float joystick_axis_maxval = 32767.0;
public:
  JoystickDroneHandler(Drone& drone, Joystick& joystick)
  : drone_(drone),
    joystick_(joystick),
    deadline_(drone.io_service()),
    heigthAdj_(0.0),
    yawAdj_(0.0)  
  {
    joystick.setButtonHandler(boost::bind(&JoystickDroneHandler::handleButton, this, _1, _2));
    joystick.setAxisHandler(boost::bind(&JoystickDroneHandler::handleAxis, this, _1, _2));
    joystick.start();
    deadline_.async_wait(boost::bind(&JoystickDroneHandler::checkDeadLine, this));
    deadline_.expires_from_now(boost::posix_time::milliseconds(50));
  }

  void checkDeadLine()
  {
    if (deadline_.expires_at() <= boost::asio::deadline_timer::traits_type::now())
    {
      if (yawAdj_ != 0.0 || heigthAdj_ != 0.0) {
        drone_.Yaw(drone_.Yaw() + yawAdj_);
        drone_.H(drone_.H() + heigthAdj_);
        drone_.SendCmd();
      }
      deadline_.expires_from_now(boost::posix_time::milliseconds(50));
    }
    deadline_.async_wait(boost::bind(&JoystickDroneHandler::checkDeadLine, this));
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
      case AxisHeigth:
        cout << "Heigth " << (value*(-1)) << endl;
        Heigth(value*(-1));
        break;
    }
    drone_.SendCmd();
  }
  
  void Roll(int16_t value) {
    drone_.Roll(deadZoneNormalize(value));
  }
  
  void Pitch(int16_t value) {
    drone_.Pitch(deadZoneNormalize(value));
  }
  
  void Yaw(int16_t value) {
    yawAdj_ = DEG2RAD(normalize(value)*5.0);
  }
  
  void Heigth(int16_t value) {
    heigthAdj_ = normalize(value)*0.2;
  }
  
  void stop() {
    cerr << "landing drone and stopping joy :-(" << endl;
    drone_.Land();
    joystick_.stop();
    drone_.io_service().stop();
  }
  
  static float deadZoneNormalize(int16_t value) {
    float val = 0.0;
    if ( ((value < 0) ? -1 * value : value * 100/32767) >= joystick_dead_zone_percent )
      val = normalize(static_cast<float>(value));
    return val;
  }
  static inline float normalize(float value)
  {
    return value/joystick_axis_maxval;
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
  signals.async_wait(boost::bind(&JoystickDroneHandler::stop, &joyHandler));
  drone.io_service().run();
}

static
std::string createLogFileName() {
  std::ostringstream strm;
  boost::posix_time::ptime now(boost::posix_time::second_clock::local_time());
  strm << boost::posix_time::to_iso_string(now) << '_' << getpid() << "_ardrone2.csv"; 
  return strm.str();
}

void runConsole(const bpo::variables_map& vm)
{
  boost::asio::io_service srv;
  Drone drone(vm["address"].as<string>(), srv);
  std::auto_ptr<UdpLogger> logger;
  
  std::string logfileName;
  if (vm.count("autolog")) {
    logfileName = createLogFileName();
  } else if (vm.count("loggfile")) {
    logfileName = vm["logfile"].as<string>();
  }

  if (!logfileName.empty()) {
    logger.reset(new UdpLogger(srv, 7778, logfileName));
    logger->start();
  }

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
      ("logfile", bpo::value<string>(), "enable logging of the csv data to arg sent by the drone")
      ("autolog", "enable logging to a <YYYYMMDDTHHMMSS>_<pid>_console.log")
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
