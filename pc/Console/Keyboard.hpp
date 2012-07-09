#ifndef __KEYBOARD_HPP__
#define __KEYBOARD_HPP__
#include <boost/function.hpp>
#include <boost/asio/posix/stream_descriptor.hpp>
#include <memory>

namespace boost{ 
  namespace asio {
    class io_service;
  }
  namespace system {
    class error_code;
  }
}

typedef boost::function<void(char)> KeyboardKeyHandler;

class Keyboard {
  boost::asio::io_service &ios_;
  boost::asio::posix::stream_descriptor input_;
  KeyboardKeyHandler buttonHandler_;
  char key_;
public:
  Keyboard(boost::asio::io_service &ios);
  ~Keyboard();

  void start();
  void stop();

  void setKeyHandler(const KeyboardKeyHandler& handler);

private:
  void startRead();
  void handleRead(const boost::system::error_code& error, size_t bytesRead);
  void setInputFlags(bool f);
};

#endif
