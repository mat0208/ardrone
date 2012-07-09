#include "Keyboard.hpp"
#include <termios.h>
#include <unistd.h>
#include <cstdlib>
#include <cstdio>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

Keyboard::Keyboard(boost::asio::io_service &ios)
: ios_(ios), input_(ios), key_(0)
{
  setInputFlags(false);
  input_.assign(::dup(STDIN_FILENO));
}

Keyboard::~Keyboard()
{
  input_.close();
  setInputFlags(true);
}


void Keyboard::start()
{
  startRead();
}

void Keyboard::stop()
{
  input_.close();
}

void Keyboard::setKeyHandler(const KeyboardKeyHandler& handler)
{
  buttonHandler_ = handler;
}


void Keyboard::startRead()
{
  input_.async_read_some(
    boost::asio::buffer(&key_, sizeof(key_)), 
    boost::bind(&Keyboard::handleRead, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
  );
}

void Keyboard::handleRead(const boost::system::error_code& error, size_t bytesRead)
{
  if (!error) {
    if (bytesRead == sizeof(key_)) {
      if (buttonHandler_) buttonHandler_(key_);
    }
    startRead();
  }
}

void Keyboard::setInputFlags(bool f) 
{
  termios t;
  tcgetattr(STDIN_FILENO, &t);
  if (f)
    t.c_lflag |= ICANON|ECHO;
  else
    t.c_lflag &= ~(ICANON|ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &t);
}
