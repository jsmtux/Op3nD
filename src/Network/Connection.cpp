#include "Connection.h"
#include <iostream>
using namespace std;
#define MTU 1400

Connection::Connection(boost::asio::io_service& io_service, short port):
        ioService(io_service), 
        socket(io_service, udp::endpoint(udp::v4(), port)),
        resetTh(&Connection::checkDeadLine,this){
    serverPort=port;
    running=true;
}

Connection::~Connection(){
    running = false;
}

void Connection::setSend(const char* ip,const char* port){
    udp::resolver resolver(ioService);
    udp::resolver::query query(udp::v4(), ip, port);
    iterator = resolver.resolve(query);
}

void Connection::Send(const char* data,const size_t size){
    socket.send_to(boost::asio::buffer(data, size), *iterator);    
}

using namespace MXML;
void Connection::Send(Tag tag, string dtd){
    static char toSend[MTU];
    DTD::newDTD(dtd);
    MEM fSend(toSend,MTU);
    BINFile xmlFile(fSend,tag,dtd);
    try{
        xmlFile.write();
        Send(toSend,fSend.getSize());
    }catch(const char* e){
        cout << "Sending failed with exception " << e<< endl;
    }
}
    
size_t Connection::receive(char* data, size_t maxSize, udp::endpoint &sender, boost::posix_time::time_duration timeOut){
    size_t size;
//    resetTimer.reset();
    size= socket.receive_from(boost::asio::buffer(data, maxSize), sender);
    return size;
}

size_t Connection::receive(udp::endpoint &sender, Tag &dRec,string dtd){
    static char toRec[MTU];
    size_t bytes;
    DTD::newDTD(dtd);
    bytes=receive(toRec,1400,sender, boost::posix_time::seconds(2));
    if(bytes==0)return bytes;
    MEM bRec(toRec,bytes);
    
    BINFile fRec(bRec,dRec);
    
    try{
    fRec.read();
    }catch(const char* e){
        cout << "error reading fRec:" << e << endl;
        exit(1);
    }
    return bytes;
}

#define RESET_RESOLUTION 500
#define RESET_TIME 2000

void Connection::checkDeadLine(){
    if(serverPort<0)return;
    while(running){
        std::this_thread::sleep_for(tMillis(RESET_RESOLUTION));
        if(resetTimer.getTicks()>tMillis(RESET_TIME)){
            resetTimer.reset();
            sendNull();
        }
    }
}

void Connection::sendNull(){
    udp::endpoint remote_endpoint=boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), serverPort);
    socket.send_to(boost::asio::buffer("", 0), remote_endpoint);
}