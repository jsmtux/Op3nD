#include "Server.h"
using namespace MXML;

#include "../Engine/Base.h"

Server::Server(boost::asio::io_service& io_service, short port):connection(io_service,port)
        ,ioService(io_service)
        ,serviceTh(&Server::runService,this)
        ,listenerTh(&Server::receive,this){
    masterState=NULL;
    masterInitState=NULL;
}

Server::~Server(){
    listenerTh.join();
    serviceTh.join();
}

void Server::stopServer(){
    running=false;
}

void Server::runService(){
    try {
        ioService.run();
    } catch( const std::exception& e ) {
        std::cerr << e.what() << std::endl;
    }
}

void Server::newClient(udp::endpoint sender){
    for(ClientStat *c: clients){
        if(c->getDir()==sender){
            return;
        }
    }
    
    cout << "Creating new client\n";
    clients.push_back(new ClientStat(sender,ioService,masterState));
}

bool Server::isServing(){
    return true;
}

void Server::updateState(){
    for(ClientStat *c:clients){
        c->update();
    }
}

bool Server::updated(){
    return false;
}

void Server::sendUpdate(){
    delete masterState;
    masterState= new State(*Base::getInstance()->getCurState());
    for(ClientStat *c:clients){
        c->send(masterState);
    }
    updateState();
}

void Server::receive(){
    running=true;
    static char data[1400];
    udp::endpoint sender;
    size_t bytes;
    while(running){
        Tag dRec;
        if(connection.receive(sender,dRec,"network.dtd")==0)continue;
        if(dRec.contains("protocol")){
            if(dRec["protocol"].contains("clientHello")){
                newClient(sender);
            }
        }
        if(dRec.contains("changeState")){
            cout << "Needs to change state\n";
            string name = dRec["changeState"]["file"].getAttrib().getString();
            StateType type = StateType(dRec["changeState"]["stateType"].getAttrib().getInt());
            string projName =dRec["changeState"]["projName"].getAttrib().getString();
            if(projName.size()!=0)
                Base::getInstance()->setProj(projName);
            Base::getInstance()->newState(name,type);
            Base::getInstance()->changeState(name,type);
            Base::getInstance()->getCurState()->loadFile();
            Base::getInstance()->beginState();
            cout << "State succesfully changed\n";
        }else if(dRec.contains("clientState")){
            for(ClientStat *c:clients){
                if(c->getDir()==sender){
                    c->newMessage(dRec);
                    break;
                }
            }
        }
    }
}

void Server::beginState(State *s){
    if(masterState!=NULL){
        delete masterState;
        delete masterInitState;
    }
    masterInitState=new State(*s);
    masterState=new State(*s);
    for(ClientStat* c:clients){
        c->beginState(s);
    }
}

void Server::endState(){
    for(ClientStat* c:clients){
        c->endState();
    }
    delete masterInitState;
    delete masterState;
    masterState=NULL;
    masterInitState=NULL;
    
}
 
bool Server::thinClient(){
    return false;
}