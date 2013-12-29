#include "Physical.h"
#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"
#include "../PhysicsWorld.h"
#include "../../ProjectManagement/MXML.h"
#include "../../ProjectManagement/Project.h"
#include "../Base.h"
#include <bullet/btBulletDynamicsCommon.h>

Physical::Physical(){
    mass=0;
    restitution=0;
    friction=0;
    angularFriction=0;
}

Physical::Physical(MXML::Tag& code){
    fromXML(code);
}

Physical::Physical(string dir){
    string name;
    if(exists(name=(Base::getInstance()->getProj()->getDir(dir)))){
       Tag data;
       XMLFile pdata(name,data,"physical.dtd");
       pdata.read();
       fromXML(data);
   }else{
        cout << "Not found";
   }
}

void Physical::fromXML(MXML::Tag &code){
  
  for(Tag t:code.getChildren()){
    if(t.getName().compare("sphere")==0){
      ShapeDefinition s;
      s.shapeCategory=S_SPHERE;
      s.size.x= code["sphere"]["radius"].getAttrib().getFloat();
      shape.push_back(s);
    }if(t.getName().compare("box")==0){
      ShapeDefinition s;
      s.shapeCategory=S_BOX;
      s.size = Vector3(code["box"]["size"]["vector"]);
      shape.push_back(s);
    }if(t.getName().compare("capsule")==0){
      ShapeDefinition s;
      s.shapeCategory=S_CAPSULE;
      s.size.x = code["capsule"]["radius"].getAttrib().getFloat();
      s.size.y = code["capsule"]["length"].getAttrib().getFloat();
      shape.push_back(s);
    }if(t.getName().compare("shape")==0){
    }
  }
  
  mass = code["mass"].getAttrib().getFloat();
  restitution = code["restitution"].getAttrib().getFloat();
  angularFriction = code["rollingFriction"].getAttrib().getFloat();
  friction = code["friction"].getAttrib().getFloat();
}

btRigidBody* Physical::toRigidBody(){    
    btCollisionShape *col=NULL;
    if(shape.size()==1){
      col=shape.front().toShape();
    }else{
     btCompoundShape* compound = new btCompoundShape();
     btTransform localTrans;
     for(ShapeDefinition s:shape){
       localTrans.setIdentity();
       localTrans.setOrigin(s.offset);
       compound->addChildShape(localTrans,s.toShape());
     }
     col=compound;
    }
    btVector3 fallInertia(0,0,0);
    col->calculateLocalInertia(mass,fallInertia);
    return new btRigidBody(mass,new btDefaultMotionState(btTransform(Quaternion(0,0,0,1),Vector3(0,0,0))),col,fallInertia);
}

using namespace MXML;

MXML::Tag Physical::toXML(){    
    Tag root("physical");
    root.addChildren(Tag("mass"));
    root.addChildren(Tag("restitution"));
    root.addChildren(Tag("friction"));
    root.addChildren(Tag("rollingFriction"));
    
    root["mass"].setAttrib(mass);
    root["restitution"].setAttrib(restitution);
    root["friction"].setAttrib(friction);
    root["rollingFriction"].setAttrib(angularFriction);
    
    for(ShapeDefinition s:shape){
      switch(s.shapeCategory){
	  case S_SPHERE:
	      root.addChildren(Tag("sphere"));
	      root["sphere"].addChildren(Tag("radius"));
	      root["sphere"]["radius"].setAttrib(s.size.x);
	      break;
	  case S_BOX:
	      root.addChildren(Tag("box"));
	      root["box"].addChildren(Tag("size"));
	      root["box"]["size"].addChildren(s.size.toXML());
	      break;
	  case S_CAPSULE:
	      root.addChildren(Tag("capsule"));
	      root["capsule"].addChildren(Tag("lenght"));
	      root["capsule"]["lenght"].setAttrib(s.size.y);
	      root["capsule"].addChildren(Tag("radius"));
	      root["capsule"]["radius"].setAttrib(s.size.x);
	      break;
      }
    }
    return root;
}

void Physical::addShape(Vector3 size, Vector3 offset, tShape category)
{
  shape.push_back(ShapeDefinition(size,offset,category));
}
