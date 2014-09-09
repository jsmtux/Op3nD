#ifndef BASICSTATEUPDATE_H
#define BASICSTATEUPDATE_H

#include "StateUpdate.h"

class BasicStateUpdate: public StateUpdate{
public:
  BasicStateUpdate();
  void init(function<void()> func);
  void pause();
private:
  void updateLoop();
  thread *updateTh;
  float updateStep;
};

#endif
