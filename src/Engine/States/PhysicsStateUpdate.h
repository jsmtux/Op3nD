#ifndef PHYSICSSTATEUPDATE_H
#define PHYSICSSTATEUPDATE_H
#include "StateUpdate.h"
#include "../PhysicsWorld.h"

class PhysicsStateUpdate: public StateUpdate{
public:
  PhysicsStateUpdate(PhysicsWorld* world);
  /**
    * Function called every time the physics world is going to be updated
    * @param world pointer to the physics world
    * @param timeStep amount of time elapsed since last call
    */
  static void physicsCallback(btDynamicsWorld *world, btScalar timeStep);
  void init(function< void > func);
  void pause();
private:
  static PhysicsStateUpdate *instance;
  void init(function<void()> func);
  PhysicsWorld *pWorld;
};

#endif