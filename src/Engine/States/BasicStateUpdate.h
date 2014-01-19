#ifndef BASICSTATEUPDATE_H
#define BASICSTATEUPDATE_H

#include "StateUpdate.h"
#define UPDATE_STEP float(1./15.)

class BasicStateUpdate: public StateUpdate{
public:
  BasicStateUpdate();
  void init(function<void()> func);
  void pause();
private:
  /**
    * Alternative to the physics callback, unused otherwise
    * TODO: update to use thread library
    * @param p
    * @return 
    */
  void updateLoop();
  thread *updateTh;
};

#endif
