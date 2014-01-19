#include "PhysicsStateUpdate.h"

PhysicsStateUpdate* PhysicsStateUpdate::instance=nullptr;

PhysicsStateUpdate::PhysicsStateUpdate(PhysicsWorld* world)
{
  pWorld=world;
}

void PhysicsStateUpdate::init(function< void() > func)
{
  StateUpdate::init(func);
  pWorld->setCallBack(physicsCallback);
  instance=this;
}

void PhysicsStateUpdate::physicsCallback(btDynamicsWorld *world, btScalar timeStep){
  instance->diffTime=instance->tUpdated.getTicks();
  instance->tUpdated.reset();
  
  if(!instance->managed){
      btCollisionObjectArray objects = world->getCollisionObjectArray();
      world->clearForces();

      for (int i = 0; i < objects.size(); i++) {
	    btRigidBody *rigidBody = btRigidBody::upcast(objects[i]);
	    if (!rigidBody) {
		continue;
	    }
	    rigidBody->applyGravity();
      }
  }
  
  instance->callback();
}

void PhysicsStateUpdate::pause()
{
  isStopped=true;
}
