#include <boost/asio.hpp>
#include <boost/format.hpp>
#include <string>
using std::string;

class Drone
{
    double Pitch_;
    double Roll_;
    double Yaw_;
    double H_;
    boost::asio::io_service ios_;
    boost::asio::ip::udp::endpoint remote_;
    boost::asio::ip::udp::socket socket_;

 public:
    Drone(const std::string& address) : Pitch_(0.0), Roll_(0.0), Yaw_(0.0), H_(0.0), ios_(), 
        remote_(boost::asio::ip::address::from_string(address), 7777) ,
        socket_(ios_)
    {
        socket_.connect(remote_);
    }
    
    static inline void safeSet(double& var, double value, double min = -1.0, double max = 1.0) {
        if (value > max) value = max;
        if (value < min) value = min;
        var = value;
    }

    void Pitch(double pitch) {
        safeSet(Pitch_, pitch);
    }

    double Pitch() {
        return Pitch_;
    }

    void Roll(double roll) {
        safeSet(Roll_, roll);
    }

    double Roll() {
        return Roll_;
    }
    

    void Yaw(double yaw) {
        safeSet(Yaw_, yaw);
    }

    double Yaw() {
        return Yaw_;
    }

    void H(double h) {
        safeSet(H_, h, 0.0, 600.0);
    }
    double H() {
        return H_;
    }

    void SendCmd()
    {
        string sendData = boost::str(boost::format("s,%1%,%2%,%3%,%4%") % Pitch_ % Roll_ % Yaw_ % H_);
        socket_.send(boost::asio::buffer(sendData));
    }
};