#include "Physical.h"
#include "../Math/Vector3.h"
#include "../Math/Quaternion.h"
#include "../PhysicsWorld.h"
#include "../../ProjectManagement/MXML.h"
#include "../../ProjectManagement/Project.h"
#include "../Base.h"

Physical::Physical(){
    mass=0;
    restitution=0;
    friction=0;
    angularFriction=0;
    size=Vector3(0,0,0);
    shapeCategory=S_UNIMPLEMENTED;
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
    if(code.contains("sphere")){
        shapeCategory=S_SPHERE;
        size.x= code["sphere"]["radius"].getAttrib().getFloat();
    }else if(code.contains("box")){
        shapeCategory=S_BOX;
        size = Vector3(code["box"]["size"]["vector"]);
    }else if(code.contains("capsule")){
        shapeCategory=S_CAPSULE;
        size.x = code["capsule"]["radius"].getAttrib().getFloat();
        size.y = code["capsule"]["length"].getAttrib().getFloat();
    }else if(code.contains("shape")){
        
        shapeCategory = S_UNIMPLEMENTED;
    }
    
    mass = code["mass"].getAttrib().getFloat();
    restitution = code["restitution"].getAttrib().getFloat();
    angularFriction = code["rollingFriction"].getAttrib().getFloat();
    friction = code["friction"].getAttrib().getFloat();
}


btRigidBody* Physical::toRigidBody(){    
    btCollisionShape *col=NULL;
    switch(shapeCategory){
        case S_SPHERE:
            col = new btSphereShape(size.x);
            break;
        case S_BOX:
            col = new btBoxShape(size);
            break;
        case S_CAPSULE:
            col = new btCapsuleShape(size.x,size.y);
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
    
    switch(shapeCategory){
        case S_SPHERE:
            root.addChildren(Tag("sphere"));
            root["sphere"].addChildren(Tag("radius"));
            root["sphere"]["radius"].setAttrib(size.x);
            break;
        case S_BOX:
            root.addChildren(Tag("box"));
            root["box"].addChildren(Tag("size"));
            root["box"]["size"].addChildren(size.toXML());
            break;
        case S_CAPSULE:
            root.addChildren(Tag("capsule"));
            root["capsule"].addChildren(Tag("lenght"));
            root["capsule"]["lenght"].setAttrib(size.y);
            root["capsule"].addChildren(Tag("radius"));
            root["capsule"]["radius"].setAttrib(size.x);
            break;
    }
    return root;
}
