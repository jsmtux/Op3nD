/* 
 * File:   Client.h
 * Author: jsmtux
 *
 * Created on January 19, 2013, 3:54 PM
 */

#ifndef CLIENT_H
#define	CLIENT_H

#include <map>
#include "Connection.h"
#include "../Engine/Timer.h"
#include "../ProjectManagement/MXML.h"
#include "../Engine/Controller.h"
#include "../Engine/States/State.h"
using namespace std;

class Client: public NetNode{
public:
    Client(boost::asio::io_service &io,const char* ip,const char* port);
    void connect();
    
    void sendUpdate();
    bool isServing();
    void updateState();
    void beginState(State *s);
    bool thinClient();
    void endState();
    bool updated();
private:
    void receive();
    void sendControllerInfo();
    
    map<int,Controller> controllerStates;
    
    Connection connection;
    thread *listener;
    bool running;
    mutex initMt;
    bool connected;
    
    Timer t;
    
    int lastAckNum;
    vector<MXML::Tag> toRead;
    mutex *msgMt;
    
    bool isUpdated;
    
    State* gameState;
    
    MXML::Tag toSend;
};

#endif	/* CLIENT_H */
