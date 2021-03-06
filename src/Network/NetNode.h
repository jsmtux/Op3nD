/* 
 * File:   NetNode.h
 * Author: jsmtux
 *
 * Created on March 5, 2013, 7:29 PM
 */

#ifndef NETNODE_H
#define	NETNODE_H

#include "../Engine/States/State.h"
class State;

class NetNode{
public:
    /**
     * called from states previous to updating state
     */
    virtual void sendUpdate()=0;
    /**
     * Returns if state needs to render
     * @return 
     */
    virtual bool isServing()=0;
    /**
     * True if node is a thin client
     */
    virtual bool thinClient()=0;
    /**
     * Applies the received updates
     */
    virtual void updateState()=0;
    /**
     * Tells the state to acknowledge update
     */
    virtual bool updated()=0;
    /**
     * When a state ends or starts the netNode needs to be informed
     */
    virtual void beginState(State* s)=0;
    virtual void endState()=0;
};

#endif	/* NETNODE_H */

