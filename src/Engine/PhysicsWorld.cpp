#include "PhysicsWorld.h"
#include "Timer.h"
#include "./Math/Vector3.h"
#include "./Graphics/GLDebugDrawer.h"

#ifndef NODRAW
GLDebugDrawer dDrawer;
#endif

PhysicsWorld::PhysicsWorld(){
    broadphase = new btDbvtBroadphase();
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0,-10,0));
#ifndef NODRAW
    dynamicsWorld->setDebugDrawer(&dDrawer);
#endif
    lastTick.reset();
    
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);
    
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));
    
    btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
    groundRigidBodyCI.m_friction=100;
    //groundRigidBodyCI.m_rollingFriction=10;
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
    
    dynamicsWorld->addRigidBody(groundRigidBody);
}

PhysicsWorld::~PhysicsWorld(){
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
    delete broadphase;
}

void PhysicsWorld::addRigidBody(btRigidBody *body){
    dynamicsWorld->addRigidBody(body);
}

#include <iostream>
using namespace std;

void PhysicsWorld::stepPhys(){
    dynamicsWorld->stepSimulation(lastTick.getTicks().count()/1000.0,10);
    lastTick.reset();
}

void PhysicsWorld::draw(){
    dynamicsWorld->debugDrawWorld();
}

void PhysicsWorld::deleteRigidBody(btRigidBody *del){
    dynamicsWorld->removeRigidBody(del);
}

void PhysicsWorld::setCallBack(btInternalTickCallback cb){
    dynamicsWorld->setInternalTickCallback(cb,static_cast<void *>(this),true);
}
