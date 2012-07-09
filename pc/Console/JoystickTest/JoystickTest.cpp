#include <iostream>
#include <cstdio>
#include <termios.h>
#include <unistd.h>
#include <boost/bind.hpp>
#include <boost/program_options.hpp>
#include "../Joystick.hpp"

using std::cin;
using std::cout;
using std::endl;
using std::cerr;
namespace bpo = boost::program_options;

void buttonEvent(uint8_t button, bool onOff)
{
  std::cout << "button(" << static_cast<unsigned>(button) << ") event: " << onOff << std::endl;
}


void axisEvent(uint8_t axis, int16_t value)
{
  std::cout << "axis(" << static_cast<unsigned>(axis) << ") value: " << value << std::endl;
}


void runJoystickTest(const bpo::variables_map& vm)
{
  boost::asio::io_service srv;
  Joystick joystick(srv, vm["joystick"].as<string>());
  joystick.setButtonHandler(boost::bind(buttonEvent, _1, _2));
  joystick.setAxisHandler(boost::bind(axisEvent, _1, _2));
  joystick.start();
  srv.run();
}

void printUsage(const string& program, const bpo::options_description& options)
{
  std::cerr << program << " [options] " << std::endl << options << std::endl;
}

int main(int argc, char **argv) {
  try {
    bpo::options_description options("Options");
    options.add_options()
      ("help,h", "print this help message")
      ("joystick", bpo::value<string>()->default_value("/dev/input/js0"), "use joystick with arg as device name")
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
    runJoystickTest(vm);
  }
  catch(const std::exception& e) {
    cerr << "exception: " << e.what() << endl;
    return 1;
  }
  return 0;
}
