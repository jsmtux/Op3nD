/**
 * @(#) Object.cpp
 */

#include "Object.h"
#include "Tile.h"
using namespace MXML;

Object::Object(){
}

Object::~Object(){
    for(Tile* t: resources){
        delete t;
    }
}

Object::Object(Tile *t){
    resources.front()=t;
    resources.front()->setParentId(id);
}

Object::Object(Object &o){
    for(Tile* t: o.resources){
        resources.push_back(new Tile(*t));
    }
}

Object::Object(MXML::Tag &code){
    fromXML(code);
}

void Object::setId(unsigned int id){
    Editable::setId(id);
    for(Tile* t: resources){
        t->setId(id);
    }
}

MXML::Tag Object::difference(Networkable &n){
    Tag ret("object");
    if(&n == NULL){
        return Tag("");
    }
    try{
        Object& o = dynamic_cast<Object&> (n);
        ret.addChildren(Tag("id"));
        ret["id"].setAttrib(Attribute(int(id)));
        for(Tile* t: resources){
            //Tag diff = visible->difference(*o.visible);
            //if(!diff.isEmpty())ret.addChildren(diff);
        }
    }catch (bad_cast& bc){
        cerr << "bad_cast in object::difference" << bc.what()<< endl;
    }
    return ret;
}

void Object::fromXML(MXML::Tag &code){
    /*if(code.contains("tile")){
        if(!resources.empty())
                resources.front()->fromXML(code["tile"]);
    }*/
}

MXML::Tag Object::toXML(){
    ;
}

Vector3 Object::getPos(){
    if(!resources.empty())
        return resources.front()->getPos();
    return Vector3(0,0,0);
}

Quaternion Object::getRot(){
    if(!resources.empty())
        return resources.front()->getRot();
    return Quaternion(0,0,0,1);
}

Vector3 Object::getSize(){
    if(!resources.empty())
        return resources.front()->getSize();
    return Vector3(0,0,0);
}

void Object::update( )
{
}

void Object::notify( )
{
	
}

void Object::draw(){
    for(Tile* t: resources){
        t->draw();
    }
}

void Object::addResource(Tile *vis){
    resources.push_back(vis);
}

void Object::setPos(Vector3 p){
    if(!resources.empty())
        resources.front()->setPos(p);
}

void Object::setRot(Quaternion r){
    if(!resources.empty())
        resources.front()->setRot(r);
}

void Object::setSize(Vector3 s){
    if(!resources.empty())
        resources.front()->setSize(s);
}

edType Object::getType(){
    return E_OBJECT;
}

void Object::setPhysical(btRigidBody* p){
    if(!resources.empty())
        resources.front()->setPhysical(p);
}

btRigidBody* Object::getPhysical(){
    if(!resources.empty())
        return resources.front()->getPhysical();
    return NULL;
}

Vector3 Object::getLinVel(){
    if(!resources.empty())
        return resources.front()->getLinVel();
}

Vector3 Object::getAngVel(){
    if(!resources.empty())
        return resources.front()->getAngVel();
}

void Object::setLinVel(Vector3 v){
    if(!resources.empty())
        resources.front()->setLinVel(v);
}

void Object::setAngVel(Vector3 v){
    if(!resources.empty())
        resources.front()->setAngVel(v);
}
