#include "ClientStat.h"
#include "../Engine/Base.h"
#include "../Engine/States/EditorState.h"
#include "../Engine/States/MeshState.h"
#include "../Engine/States/PlayState.h"

using namespace MXML;

ClientStat::ClientStat(udp::endpoint address,boost::asio::io_service& ioService,State *s){
    dir = address;
    connection = new Connection(ioService,-1);
    msgMt=new mutex();
    
    stringstream ss;
    ss << address.port();
    connection->setSend(address.address().to_string().c_str(),string(ss.str()).c_str());
    Tag conn("connection");
    conn.addChildren(Tag("protocol"));
    conn["protocol"].addChildren(Tag("serverHello"));
    conn["protocol"]["serverHello"].setAttrib(1);
    connection->Send(conn,"network.dtd");
    nSent=0;
    lastAckNum=-1;
    lastAck=new State(*s);
    controllerNo=Base::getInstance()->addController(new Controller);
}

void ClientStat::send(State *g){
    Tag msg("connection");
    msg.addChildren(Tag("id"));
    msg["id"].setAttrib(Attribute(nSent));
    Tag diff=lastAck->difference(*g);
    if(diff.isEmpty()){
        return;
    }
    cout << "Master state differs\n";
    msg.addChildren(diff);
    sent[nSent%CLIENT_SNAPSHOTS]=diff;
    sentNum[nSent%CLIENT_SNAPSHOTS]=nSent;
    connection->Send(msg,"network.dtd");
    nSent++;
}

udp::endpoint ClientStat::getDir(){
    return dir;
}

void ClientStat::newMessage(Tag &msg){
    msgMt->lock();
    if(msg.contains("ack")){
        confirmState(msg["ack"].getAttrib().getInt());
    }
    toRead.push_back(msg["clientState"]);
    msgMt->unlock();
}

void ClientStat::update(){
    msgMt->lock();
    for(Tag t:toRead){
        if(t.contains("controller")){
            Base::getInstance()->getControllers()[controllerNo]->fromXML(t["controller"]);
        }
    }
    toRead.clear();
    msgMt->unlock();
}

void ClientStat::confirmState(int num){
    if(num>lastAckNum){
        lastAckNum=num;
        for(int i=0;i<CLIENT_SNAPSHOTS;i++){
            if(sentNum[i]==num){
                lastAck->fromXML(sent[i]);
            }
        }
    }
}

void ClientStat::endState(){
    delete lastAck;
    lastAck=NULL;
    for(int i=0;i<CLIENT_SNAPSHOTS;i++){
        sentNum[i]=-1;
    }
}

void ClientStat::beginState(State *s){
    lastAck=new State(*s);
    nSent=0;
    lastAckNum=-1;
}