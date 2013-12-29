/* 
 * File:   Physics.h
 * Author: jsmtux
 *
 * Created on October 30, 2012, 9:59 AM
 */

#ifndef PHYSICS_H
#define	PHYSICS_H
#include <btBulletDynamicsCommon.h>
#define UPDATE_STEP float(1./15.)
#include "Timer.h"
#include <vector>

enum colType{C_SPHERE=0, C_BOX, C_CAPSULE, C_SHAPE};

class PhysicsWorld{
private:
    btBroadphaseInterface* broadphase;
    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;
    Timer lastTick;
public:
    void addRigidBody(btRigidBody *body);
    void deleteRigidBody(btRigidBody *del);
    void setCallBack(btInternalTickCallback cb);
    void stepPhys();
    void draw();
    std::vector<void*> contactTest(btRigidBody* body);
    PhysicsWorld();
    ~PhysicsWorld();
};


#endif	/* PHYSICS_H */

