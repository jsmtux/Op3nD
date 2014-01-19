/**
 * @(#) Base.cpp
 */
#include "Base.h"


#include "../ProjectManagement/Project.h"

#ifdef NOGUI
#include "Graphics/RenderingSFML.h"
#endif

Base* Base::instance= new Base();

Base::Base(){
    rc=NULL;
    currentProj=NULL;
}

StateManager* Base::getStateManager()
{
  return &stateManager;
}

void Base::setProj(Project* p){
    endProj();
    if(currentProj!=NULL){
      cout << "Must end project before starting new one" << endl;
      assert(false);
    }
    currentProj=p;
}

void Base::endProj(){
    if(currentProj!=NULL){
        delete currentProj;
        currentProj=NULL;
        stateManager.clearStates();
    }    
}

Base* Base::getInstance(){
    return instance;
}

Project* Base::getProj(){
    return currentProj;
}

void Base::end(){
    endProj();
    delete currentProj;
}

int Base::addController(Controller* c){
    controllers.push_back(c);
    return controllers.size()-1;
}

vector<Controller*> &Base::getControllers(){
    return controllers;
}

void Base::setNode(NetNode* n){
    //netNode=n;
}

void Base::setRC(RenderingContext* rc)
{
  this->rc=rc;
}

RenderingContext* Base::getRC()
{
  return rc;
}
