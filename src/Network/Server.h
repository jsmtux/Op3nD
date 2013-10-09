#ifndef _SERVER_H
#define _SERVER_H

#include <vector>
using namespace std;

#include "../Engine/Controller.h"
#include "../Engine/Controller.h"
#include "ClientStat.h"
#include "../Network/Connection.h"
#include "../ProjectManagement/MXML.h"
#include "../Engine/States/State.h"
#include "NetNode.h"

class Server: public NetNode{
public:
    /**
     * Starts receiving
     * @param io_service
     * @param port port to send
     */
    Server(boost::asio::io_service& io_service, short port);
    /**
     * Sends master state to clients
     */
    void sendUpdate();
    void stopServer();
    ~Server();
    bool isServing(); 
    bool thinClient();
    /**
     * Processes clients' received messages
     */
    void updateState();
    void beginState(State *s);
    void endState();
    bool updated();
private:
    void newClient(udp::endpoint sender);
    /**
     * Checks for connect and delegates everything else
     */
    void receive();
    /**
     * runs ioservice
     */
    void runService();
    
    vector<Controller*> controllers;
    State* masterState;
    State* masterInitState;
    vector<ClientStat*> clients;
    
    boost::asio::io_service& ioService;
    Connection connection;
    
    Timer t;
    thread listenerTh;
    thread serviceTh;
    bool running;
};

#endif