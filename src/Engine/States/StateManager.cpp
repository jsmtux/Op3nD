#include "StateManager.h"

#include "EditorState.h"
#include "MeshState.h"
#include "PlayState.h"

#include <assert.h>

StateManager::StateManager()
{
  netNode=NULL;
}

int StateManager::newState( State* newSt){
  changeStateMt.lock();
  if(!states.empty()){
    if(netNode)netNode->endState();
    states.top()->pause();
  }
  states.push(newSt);
  beginState();
  changeStateMt.unlock();
}

void StateManager::deleteState(){
  if(states.empty()){
    return;
  }
  changeStateMt.lock();
  states.top()->clear();
  states.pop();
  states.top()->updateResolution();
  beginState();
  changeStateMt.unlock();
}


void StateManager::clearStates(){
  while(!states.empty()){
    deleteState();
  }
}

bool StateManager::iteration()
{
  if(states.empty()){
    return false;
  }
  states.top()->iteration();
  //Check if state needs to be changed and change it
  changeStateMt.lock();
  changeStateMt.unlock();
  return true;
}

State* StateManager::getCurState()
{
  if(!states.empty())
    return states.top();
  else
    return nullptr;
}

void StateManager::beginState()
{
  if(netNode){
      states.top()->setNetNode(netNode);
      netNode->beginState(states.top());
  }
  states.top()->beginUpdateLoop();
}
