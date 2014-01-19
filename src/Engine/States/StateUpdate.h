#ifndef STATEUPDATE_H
#define STATEUPDATE_H

#include <chrono>
#include <mutex>
#include <thread>
using namespace std;
#include "../Timer.h"

class StateUpdate{
public:
  virtual void init(std::function<void()> func);
  void lockUpdate();
  void unlockUpdate();
  tMillis getDiffTime();
  bool getStopped();
  virtual void pause()=0;
protected:
  StateUpdate();
  function<void()> callback;
  bool isStopped;
  Timer tUpdated;
  tMillis diffTime;
  mutex *updateLock;
  //True when it is a client
  bool managed;
};

#endif
