#include "StateUpdate.h"
#include <iostream>
using namespace std;

StateUpdate::StateUpdate()
{
  callback=[](){};
  isStopped=true;
  updateLock= new mutex;
  diffTime= chrono::duration_cast<tMillis>(chrono::seconds(0));
  bool managed = false;
}

bool StateUpdate::getStopped()
{
  return isStopped;
}

void StateUpdate::init(function< void() > func )
{
  if(!isStopped){
      cout << "Trying to start already started state\n";
      return;
  }
  isStopped=false;
  callback=func;
}

tMillis StateUpdate::getDiffTime(){
    return diffTime;
}

void StateUpdate::lockUpdate(){
    updateLock->lock();
}

void StateUpdate::unlockUpdate(){
    updateLock->unlock();
}