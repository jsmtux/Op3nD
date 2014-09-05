/* 
 * File:   Physical.h
 * Author: jsmtux
 *
 * Created on December 18, 2012, 2:16 PM
 */

#ifndef PHYSICAL_H
#define	PHYSICAL_H

#include "../../ProjectManagement/MXML.h"
#include "Math/Vector3.h"
#include "Resources/Resource.h"
#include <map>

class Timer;

enum tShape{S_BOX,S_SPHERE,S_CAPSULE,S_UNIMPLEMENTED};

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
    float getMass(){return mass;}
    float getRestitution(){return restitution;}
    float getFriction(){return friction;}
    float getAngularFriction(){return angularFriction;}
    void addShape(Vector3 size, Vector3 offset, tShape category);
    void setContactResponse(bool responds);
    bool getContactResponse();
private:
    float mass;
    float restitution;
    float friction;
    float angularFriction;
    bool contactResponse=true;
    struct ShapeDefinition{
      ShapeDefinition(){};
      ShapeDefinition(Vector3 size, Vector3 offset, tShape shape):size(size), offset(offset), shapeCategory(shape){};
      Vector3 size;
      Vector3 offset;
      tShape shapeCategory;
      btCollisionShape* toShape(){
	switch(shapeCategory){
	  case S_SPHERE:
	    return new btSphereShape(size.x);
	    break;
	  case S_BOX:
	    return new btBoxShape(size);
	    break;
	  case S_CAPSULE:
	    return new btCapsuleShape(size.x,size.y);
	}
      }
    };
    vector<ShapeDefinition> shape;
};

#endif	/* PHYSICAL_H */

