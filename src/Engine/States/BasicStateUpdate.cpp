#include "BasicStateUpdate.h"
#include "../Base.h"

BasicStateUpdate::BasicStateUpdate()
{
  isStopped=true;
  updateTh = NULL;
  updateStep = 1./15.;
}

void BasicStateUpdate::init(function< void() > func)
{
  StateUpdate::init(func);
  updateTh=new thread(&BasicStateUpdate::updateLoop,this);
}

void BasicStateUpdate::updateLoop(){
    while(!isStopped){
        diffTime=tUpdated.getTicks();
        tUpdated.reset();
        
        updateLock->lock();
        callback();
	
	for(Controller* c:Base::getInstance()->getControllers()){
	  c->finishIteration();
	}
        updateLock->unlock();
        
        tMillis sl=std::chrono::milliseconds(int(updateStep*1000))- tUpdated.getTicks();
        std::this_thread::sleep_for(sl);
    }
}

void BasicStateUpdate::pause()
{
  isStopped=true;
  if(updateTh)
      updateTh->join();
  delete updateTh;
  updateTh = NULL;
}
