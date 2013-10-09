/**
 * @(#) Base.h
 */

#ifndef BASE_H_H
#define BASE_H_H

#ifndef ANDROID
#include <GL/glew.h>
#include <SFML/OpenGL.hpp>
#else
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#endif

#include "./States/State.h"


#include "Controller.h"
#include "Math/Vector3.h"
#include "../Network/NetNode.h"
#include "Graphics/RenderingContext.h"

class Project;

#include <vector>
#include <mutex>
#include <string>
using namespace std;
/**
 * Base class of the engine, holds state, rendering context and project information
 */
class Base
{	
public:
    /**
     * Begins current state
     * It must be initialised, otherwise prints error
     */
    static void beginState();
    static void stopState();
    /**
     * Changes current state
     * @param name new state's name
     */
    void changeState( string name, StateType t );
    /**
     * Creates a new state, note that it must be initialised before running it with beginState
     * @param name name of the new state
     * @return returns the id of the state created
     */
    int newState( string name, StateType t );
    /**
     * Deletes state with name name
     * @param name of the state it is going to delete
     */
    void deleteState( string name , StateType t);
    /**
     * returns a pointer to the current state
     * @return 
     */
    State* getCurState();
    /**
     * Singleton, returns its object
     * @return singleton object
     */
    static Base* getInstance();
    /**
     * returns the current project
     * @return current project
     */
    Project *getProj();
    /**
     * Changes the project currently used.
     * It must exist within ./projs
     * @param name name of the new project
     */
    void setProj(string dir);
    /**
     * Closes current project
     */
    void endProj();
    /**
     * Initialises the engine
     */
    void init();
    /**
     * Finishes the engine
     */
    void end();
    /**
     * Adds a new controller
     * @param c the controller to be added
     * @return id assigned to the controller
     */
    int addController(Controller *c);
    /**
     * returns the controller list
     * @return the controller list
     */
    vector<Controller*> &getControllers();
    Vector3 getResolution();
    /**
     * Adds a connection either server or client
     * Resets the current project
     */
    void setNode(NetNode* n);
    void gameLoop();
    void clearStates();
    void setRC(RenderingContext* rc);
    void setResolution(Vector3 res);
    float getRatio();
private:
    Base();
    static Base* instance;
    int getState(string name, StateType t);
    Project * currentProj;
    State * currentState;
    vector<State*> states;
    vector<Controller*> controllers;
    Vector3 resolution;
    NetNode *netNode;
    mutex changeStateMt;
    RenderingContext *rc;
    float ratio;
};

#endif
