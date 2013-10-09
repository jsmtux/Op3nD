#ifndef _CLIENT_STAT_H
#define _CLIENT_STAT_H

#define CLIENT_SNAPSHOTS 10
#include <thread>

#include "../Engine/Controller.h"
#include "../Engine/States/State.h"
#include "../Network/Connection.h"
using namespace MXML;

class ClientStat{
public:
    ClientStat(udp::endpoint address,boost::asio::io_service& ioService, State *s);
    /**
     * Sends the difference between the last ack'd state and the current
     * @param g
     */
    void send(State *g);
    udp::endpoint getDir();
    /**
     * receives a new message, it will be processed later
     * acks in real time
     * @param msg 
     */
    void newMessage(Tag &msg);
    /**
     * processes messages received
     */
    void update();
    void endState();
    void beginState(State *s);
private:
    void confirmState(int num);
    udp::endpoint dir;
    //last acknowledged state
    State *lastAck;
    int lastAckNum;
    //keep track of sent id
    int nSent;
    //sent info
    Tag sent[CLIENT_SNAPSHOTS];
    int sentNum[CLIENT_SNAPSHOTS];
    
    int controllerNo;
    
    Connection *connection;
    vector<Tag> toRead;
    
    mutex *msgMt;
};

#endif
