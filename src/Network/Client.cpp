#include "Client.h"
#include "../Engine/Base.h"
#include "../ProjectManagement/Project.h"
#ifndef ANDROID
#include <SFML/System.hpp>
#endif
#include <boost/asio.hpp>
using boost::asio::ip::udp;
using namespace std;
using namespace MXML;

Client::Client(boost::asio::io_service &io,const char* ip, const char* port):connection(io,1314){
    connection.setSend(ip,port);
    isUpdated=false;
    msgMt=new mutex();
    lastAckNum=0;
    connected=false;
    gameState=NULL;
}

void Client::connect(){
    listener=new thread(&Client::receive,this);
    
    while(!connected){
        cout << "sending connection\n";
        Tag conn("connection");
        conn.addChildren(Tag("protocol"));
        conn["protocol"].addChildren(Tag("clientHello"));
        conn["protocol"]["clientHello"].setAttrib(1);
        connection.Send(conn,"network.dtd");
        initMt.lock();
    }
}

void Client::updateState(){
    msgMt->lock();
    for(Tag r:toRead){
        cout << "going to update\n";
        gameState->fromXML(r);
        cout << "state updated\n";
    }
    toRead.clear();
    msgMt->unlock();
}

void Client::receive(){
    running=true;
    udp::endpoint sender;
    cout << "Receiving\n";
    while(running){
        Tag received;
        size_t size=connection.receive(sender,received,"network.dtd");
        initMt.unlock();
        
        if(!size)continue;
        
        if(received.contains("protocol")){
            if(received["protocol"].contains("serverHello")){
                connected=true;
                //Base::getInstance()->getProj()->setRemoteProjs();
            }
        }
        if(received.contains("id")){
            Tag ack("ack");
            int ackNum=received["id"].getAttrib().getInt();
            ack.setAttrib(ackNum);
            toSend.addChildren(ack);
            if(ackNum<lastAckNum){
                continue;
            }else{
                lastAckNum=ackNum;                
            }
        }
        if(received.contains("gameState")){
            msgMt->lock();
            toRead.push_back(received["gameState"]);
            isUpdated=true;
            msgMt->unlock();
            cout << "Received gameState\n";
        }
    }
}

void Client::sendControllerInfo(){
    Tag send("connection");
    send.addChildren(Tag("clientState"));
    if(gameState!=NULL)
        send["clientState"].addChildren(Base::getInstance()->getControllers()[0]->toXML());
    
    for(Tag t:toSend.getChildren()){
        send.addChildren(t);
    }
    toSend.clear();
    connection.Send(send,"network.dtd");
}

bool Client::isServing(){
    return false;
}

void Client::sendUpdate(){
    sendControllerInfo();
}

void Client::beginState(State *s){
    gameState=s;
    Tag send("changeState");
    send.addChildren(Tag("file"));
    send["file"].setAttrib(Attribute(s->getName().c_str()));
    send.addChildren(Tag("stateType"));
    send["stateType"].setAttrib(Attribute(s->getType()));
    send.addChildren(Tag("projName"));
    send["projName"].setAttrib(Attribute(Base::getInstance()->getProj()->getName().c_str()));
    toSend.addChildren(send);
}

bool Client::thinClient(){
    return true;
}

void Client::endState(){;}

bool Client::updated(){
    bool tmp=isUpdated;
    isUpdated=false;
    return tmp;
}
