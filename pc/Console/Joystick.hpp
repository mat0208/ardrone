#ifndef __JOYSTICK_HPP__
#define __JOYSTICK_HPP__
#include <boost/function.hpp>
#include <boost/cstdint.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <string>
#include <memory>

using std::string;
using std::auto_ptr;
using boost::uint8_t;
using boost::int16_t;

namespace boost{ 
  namespace asio {
    class io_service;
  }
  namespace system {
    class error_code;
  }
}

struct js_event;

typedef boost::function<void(uint8_t, bool)> JoystickButtonHandler;
typedef boost::function<void(uint8_t, int16_t)> JoystickAxisHandler;

class Joystick {
  boost::asio::io_service &ios_;
  boost::asio::posix::stream_descriptor jsStream_;
  string deviceName_;
  JoystickButtonHandler buttonHandler_;
  JoystickAxisHandler axisHandler_;
  auto_ptr<js_event> jsEventBuff_;

public:
  Joystick(boost::asio::io_service &ios, const string& deviceName);
  ~Joystick();

  void start();
  void stop();

  void setButtonHandler(const JoystickButtonHandler& handler);
  void setAxisHandler(const JoystickAxisHandler& handler);

private:
  void startRead();
  void handleRead(const boost::system::error_code& error, size_t bytesRead);
  void handleJoystickEvent(const js_event& ev);
};

#endif
