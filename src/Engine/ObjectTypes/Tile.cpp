/**
 * @(#) Tile.cpp
 */

#include "Tile.h"
#include "Editable.h"
#include "Resource.h"
#include "../Base.h"
#include "Image.h"
#include "Physical.h"
#include "../Graphics/Shading.h"
#include "../Math/Matrix.h"
#include </home/jsmtux/projects/Op3nD/src/Engine/PhysicsWorld.h>
#include "../../ProjectManagement/Project.h"

extern Image* imtmp;

#ifndef NODRAW
GLuint Tile::VBO;
GLuint Tile::IBO;
#endif

Tile::Tile(Vector3 p, Vector3 s, Quaternion r, string dirResource){
  position=p;
  size=s;
  rotation=r;
  physInfo=NULL;
  resource=NULL;
  if(!dirResource.empty())
    setResource(dirResource);
}

Tile::Tile(Tile &tile):Editable(tile.id){
  colored=tile.colored;
  string resourceName=tile.resource->getName();
  resource = NULL;
  if(!resourceName.empty())
    setResource(resourceName);
  
  physInfo=NULL;
  position=tile.position;
  size=tile.size;
  rotation=tile.rotation;
}

Tile::Tile(MXML::Tag &code){
  physInfo=NULL;
  resource=NULL;
  fromXML(code);
}

Tile::~Tile(){    
  if(physInfo)Base::getInstance()->getCurState()->deleteRigidBody(physInfo);
  if(resource)
    resource->free();
}

void Tile::load(){
  ;
}

MXML::Tag Tile::difference(Networkable &n){
  Tag ret("tile");
  if(&n == NULL){
    return Tag("");
  }
  try{
    Tile& t = dynamic_cast<Tile&> (n);
    if(t.position!=position){
      ret.addChildren(Tag("position"));
      ret["position"].addChildren(t.position.toXML());
    }
    if(t.rotation!=rotation){
      ret.addChildren(Tag("rotation"));
      ret["rotation"].addChildren(t.rotation.toXML());
    }
    if(!ret.isEmpty()){
      ret.addChildren(Tag("id"));
      ret["id"].setAttrib(Attribute(int(id)));
    }
  }catch (bad_cast& bc){
    cerr << "bad_cast in tile::difference" << bc.what()<< endl;
  }
  return ret;    
}

Vector3 Tile::getPos(){
  return position;
}

Vector3 Tile::getSize(){
  return size;
}

Quaternion Tile::getRot(){
  return rotation;
}

void Tile::updateMotionState(){
  if(physInfo){
    btMotionState* tmp=physInfo->getMotionState();
    btTransform trans;
    tmp->getWorldTransform(trans);
    physInfo->setMotionState(new btDefaultMotionState(btTransform(rotation,position)));
  }
}

void Tile::setPos(Vector3 p){
  position = p;
  updateMotionState();
}

void Tile::setRot(Quaternion r){
  rotation=r;
  updateMotionState();
}

void Tile::setSize(Vector3 s){
  size =s;
  updateMotionState();
}

void Tile::setLinVel(Vector3 v){
  if(physInfo){        
    physInfo->activate();
    physInfo->setLinearVelocity(v);
  }
}

void Tile::setAngVel(Vector3 v){
  if(physInfo){
    physInfo->activate();
    physInfo->applyTorque(v);
  }
}

Vector3 Tile::getLinVel(){
  if(physInfo)
    return physInfo->getLinearVelocity();
  return Vector3(0,0,0);
}

Vector3 Tile::getAngVel(){
  if(physInfo)
    return physInfo->getAngularVelocity();
  return Vector3(0,0,0);
}

void Tile::draw( ){
  #ifndef NODRAW
  Shading::getActive()->setObjInd(getId());
  if(resource)
    Shading::getActive()->setObjMat(getTrans()*resource->getTransform());
  #endif
  if(physInfo){
    btTransform trans= physInfo->getWorldTransform();
    position=Vector3(trans.getOrigin());
    rotation=Quaternion(trans.getRotation());
  }
  if(resource){
    resource->Draw();
  }
}

void Tile::fromXML(MXML::Tag &code){
  if(code.contains("position")){
    position.fromXML( code["position"]["vector"]);
  }
  if(code.contains("rotation"))
    rotation.fromXML(code["rotation"]["quaternion"]);
  if(physInfo){
    btTransform trans;
    trans.setOrigin(position);
    trans.setRotation(rotation);
    physInfo->setWorldTransform(trans);
  }
  if(code.contains("size"))size.fromXML(code["size"]["vector"]);

  if(code.contains("image"))setResource(code["image"].getAttrib().getString());
}

using namespace MXML;

void Tile::setResource(string dirResource){
  if(resource){
    cerr << "Adding resource to already resourced\n\n";
    return;
  }
  resource=Resource::Load(dirResource);
  string name;
  if(exists(name=(Base::getInstance()->getProj()->getDir(dirResource)+".xml"))){
    Tag data;
    XMLFile pdata(name,data,"physical.dtd");
    pdata.read();
    setPhysical(data);
  }
}

MXML::Tag Tile::toXML(){
  Tag ret("tile");
  ret.addChildren(Tag("position"));
  ret.addChildren(Tag("rotation"));
  ret.addChildren(Tag("size"));
  ret.addChildren(Tag("image"));
  ret.addChildren(Tag("id"));
  
  ret["id"].setAttrib(Attribute(int(id)));
  ret["position"].addChildren(position.toXML());
  ret["rotation"].addChildren(rotation.toXML());
  ret["size"].addChildren(size.toXML());
  ret["image"].setAttrib(Attribute(string(resource->getName()).c_str()));
  return ret;
}

edType Tile::getType(){
  return E_TILE;
}

void Tile::setPhysical(btRigidBody* p){
  if(physInfo)Base::getInstance()->getCurState()->deleteRigidBody(physInfo);
					Vector3 tPos=position;
  Quaternion tRot=rotation;
  physInfo = p;
  if(p){
    p->setActivationState(DISABLE_DEACTIVATION);
    State *tmp=Base::getInstance()->getCurState();
    if(tmp)
      tmp->addRigidBody(physInfo);
    setPos(tPos);
    setRot(tRot);
    physInfo->getCollisionShape()->setLocalScaling(size);
  }
  physInfo->setUserPointer(this);
}

void Tile::setPhysical(MXML::Tag &code){
  setPhysical(Physical(code).toRigidBody());
}

btRigidBody* Tile::getPhysical(){
  return physInfo;
}

Matrix Tile::getTrans(){
  return position.toPositionMatrix() * rotation.toRotationMatrix() * size.toScaleMatrix();
}

Resource* Tile::getResource()
{
  return resource;
}

std::vector< Tile* > Tile::getColliding()
{
  vector<void*> pointers=Base::getInstance()->getCurState()->getPhysicsWorld()->contactTest(physInfo);
  vector<Tile*> ret;
  
  for(void* p:pointers){
    ret.push_back(static_cast<Tile*>(p));
  }
  
  return ret;
}
