#include "Joystick.hpp"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <linux/joystick.h>

Joystick::Joystick(boost::asio::io_service &ios, const string& deviceName)
: ios_(ios), deviceName_(deviceName), jsStream_(ios), jsEventBuff_(new js_event)
{
  jsStream_.assign(::open(deviceName.c_str(), O_NONBLOCK | O_RDONLY));
}

Joystick::~Joystick()
{
}


void Joystick::start()
{
  startRead();
}

void Joystick::stop()
{
  jsStream_.close();
}

void Joystick::setButtonHandler(const JoystickButtonHandler& handler)
{
  buttonHandler_ = handler;
}

void Joystick::setAxisHandler(const JoystickAxisHandler& handler)
{
  axisHandler_ = handler;
}

void Joystick::startRead()
{
  jsStream_.async_read_some(
    boost::asio::buffer(jsEventBuff_.get(), sizeof(js_event)), 
    boost::bind(&Joystick::handleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
  );
}

void Joystick::handleRead(const boost::system::error_code& error, size_t bytesRead)
{
  if (!error) {
    if (bytesRead == sizeof(js_event)) {
      handleJoystickEvent(*jsEventBuff_);
    }
    startRead();
  }
}

void Joystick::handleJoystickEvent(const js_event& ev)
{
  if (ev.type & JS_EVENT_INIT) return;
  if (ev.type & JS_EVENT_BUTTON) {
    if (buttonHandler_) buttonHandler_(ev.number, !!ev.value);
  } else if (ev.type & JS_EVENT_AXIS) {
    if (axisHandler_) axisHandler_(ev.number, ev.value);    
  }
}
