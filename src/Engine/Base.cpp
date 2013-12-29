/**
 * @(#) Base.cpp
 */
#include "Base.h"


#include "../ProjectManagement/Project.h"

#include "./States/EditorState.h"
#include "./States/MeshState.h"
#include "./States/PlayState.h"
#ifdef NOGUI
#include "Graphics/RenderingSFML.h"
#endif

Base* Base::instance= new Base();

Base::Base(){
    rc=NULL;
    currentState=NULL;
    currentProj=NULL;
    netNode=NULL;
    setResolution(Vector3(640,480,32));
}

void Base::init(){
#ifndef NODRAW    
#ifdef NOGUI
    rc = new RenderingSFML();   
    rc->init();
#endif
#endif
}

void Base::setProj(string dir){
    endProj();
    if(currentProj!=NULL){
        if(currentProj->getDir().compare(dir)==0)return;
        delete currentProj;
        currentProj=NULL;
    }
    if(dir.empty()){
      cerr << "Trying to change project to an empty directory"<< endl;
      return;
    }
    currentProj=new Project(dir);
    currentProj->read();
}

void Base::clearStates(){
        for(State* s:states){
            s->stop();
            s->clear();
        }
        states.clear();
        currentState=NULL;    
}

void Base::endProj(){
    if(currentProj!=NULL){
        delete currentProj;
        currentProj=NULL;
        clearStates();
    }    
}

void Base::beginState( ){
    if(currentState==NULL){
        cout << "State not initialized\n";
        return;
    }
    if(netNode){
        currentState->setNetNode(netNode);
        netNode->beginState(currentState);
    }
    currentState->beginUpdateLoop();
}

void Base::stopState( ){
    if(currentState==NULL){
        cout << "State not initialized\n";
        return;
    }
    currentState->stop();
    if(!currentState->reusable()){
        for(int i=0;i<states.size();i++){
            if(states[i]==currentState)
                states.erase(states.begin()+i);
        }
    }
}

int Base::newState( string name, StateType t){
    if(getState(name,t)!=-1)return -1;
    switch(t){
        case EMPTYST:
            states.push_back(new State(name));
            break;
        case GAMEST:
            states.push_back(new PlayState(name));
            break;
        case MESHST:
            states.push_back(new MeshState(name));
            break;
        case EDITORST:
            states.push_back(new EditorState(name));
    }
}

void Base::changeState( string name, StateType t ){
    changeStateMt.lock();
    cout << "Changing state\n";
    if(currentState!=NULL){
        if(currentState->getName()==name&&currentState->getType()==t)return;
        if(netNode)netNode->endState();
        currentState->stop();
    }
    int it=getState(name,t);
    if(it==-1){
        cout << "state " << name << " not initialised";
        return;
    }
    currentState = (states.at(it));
    cout << "State changed\n";
    
    changeStateMt.unlock();
}

void Base::deleteState( string name , StateType t){
    states.erase(states.begin() + getState(name, t));
}

State* Base::getCurState(){
    return currentState;
}

int Base::getState(string name, StateType t){
    int i;
    for(i=0;i<states.size();i++){
        if(states[i]->getName()==name&&states[i]->getType()==t)
            return i;
    }
    return -1;
}

Base* Base::getInstance(){
    return instance;
}

Project* Base::getProj(){
    return currentProj;
}

void Base::end(){
    for(int i=0;i<states.size();i++){
        states[i]->stop();
        delete states[i];
    }
    delete currentProj;
}

int Base::addController(Controller* c){
    controllers.push_back(c);
    return controllers.size()-1;
}

vector<Controller*> &Base::getControllers(){
    return controllers;
}

Vector3 Base::getResolution(){
    return resolution;
}

void Base::setNode(NetNode* n){
    netNode=n;
}

void Base::gameLoop(){
    while(currentState){
        currentState->iteration();
	//Check if state needs to be changed and change it
        changeStateMt.lock();
        changeStateMt.unlock();
        if(rc){
            rc->update();
        }
    }
}

void Base::setRC(RenderingContext* rc)
{
  cout << "Setting rc"<< endl;
  if(rc!=NULL){
    cerr << "Trying to set rendering context to already initialised base\n";
    //exit(1);
  }
  this->rc=rc;
}

void Base::setResolution(Vector3 res)
{
  this->resolution=res;
  ratio=res.x/res.y;
  for(State* s:states){
    s->changeResolution();
  }
}

float Base::getRatio()
{
  return ratio;
}
