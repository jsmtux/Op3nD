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
#include "States/StateManager.h"


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
    * Singleton, returns its object
    * @return singleton object
    */
  static Base* getInstance();
  /**
    * Finishes the engine
    */
  void end();
  
  StateManager* getStateManager();
  
  //Project//
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
  void setProj(Project* p);
  /**
    * Closes current project
    */
  void endProj();
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
  
  void setRC(RenderingContext* rc);
  RenderingContext* getRC();
  
  /**
    * Adds a connection either server or client
    * Resets the current project
    */
  void setNode(NetNode* n);///////TODO Fix
private:
  Base();
  static Base* instance;
  StateManager stateManager;
  RenderingContext *rc;
  Project * currentProj;
  vector<Controller*> controllers;
};

#endif

