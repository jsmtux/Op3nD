#ifndef _CONNECTION_H
#define _CONNECTION_H

#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include "../Engine/Timer.h"
#include "../ProjectManagement/MXML.h"
#include <thread>
using boost::asio::ip::udp;

typedef boost::function<void(const boost::system::error_code& error, size_t bytes_recvd)> receiveFunction;

class Connection{
public:
    Connection(boost::asio::io_service& io_service,short port);
    ~Connection();
    void setSend(const char* ip,const char* port);
    void Send(const char* data,const size_t size);
    size_t receive(char* data, size_t maxSize ,udp::endpoint &sender, boost::posix_time::time_duration timeOut);
    void Send(MXML::Tag tag,string dtd);
    size_t receive(udp::endpoint &sender, MXML::Tag &dRec,string dtd);
private:
    void checkDeadLine();
    void sendNull();
    short serverPort;
    int step;
    bool running;
    Timer resetTimer;
    
    boost::asio::io_service& ioService;
    udp::socket socket;
    udp::endpoint senderEndpoint;
    udp::resolver::iterator iterator;
    thread resetTh;
    mutex resetMt;
};

#endif
