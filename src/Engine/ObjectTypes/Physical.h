/* 
 * File:   Physical.h
 * Author: jsmtux
 *
 * Created on December 18, 2012, 2:16 PM
 */

#ifndef PHYSICAL_H
#define	PHYSICAL_H

#include "../../ProjectManagement/MXML.h"
#include "../Math/Vector3.h"
#include "Resource.h"
#include <map>

class Timer;

enum tShape{S_BOX,S_SPHERE,S_CAPSULE,S_COMPOUND,S_UNIMPLEMENTED};

class Physical{
public:
    Physical();
    Physical(MXML::Tag &code);
    Physical(string dir);
    btRigidBody* toRigidBody();
    void fromXML(MXML::Tag &code);
    MXML::Tag toXML();
    void setMass(float m){mass=m;}
    void setRestitution(float r){restitution=r;}
    void setFriction(float f){friction=f;}
    void setAngularFriction(float a){angularFriction=a;}
    void setSize(Vector3 s){size=s;}
    void setShape(tShape s){shapeCategory=s;}
    float getMass(){return mass;}
    float getRestitution(){return restitution;}
    float getFriction(){return friction;}
    float getAngularFriction(){return angularFriction;}
    Vector3 getSize(){return size;}
    tShape getShape(){return shapeCategory;}
private:
    float mass;
    float restitution;
    float friction;
    float angularFriction;
    Vector3 size;
    tShape shapeCategory;
};

#endif	/* PHYSICAL_H */

