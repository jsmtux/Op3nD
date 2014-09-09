/**
 * @(#) Controller.cpp
 */

#include "Controller.h"
#include <cmath>

#include "lua_typed_enums.h"
using namespace MXML;

int Controller::contNum =0;

Controller::Controller(){
  id=contNum++;
  
  for(int i=0;i<14;i++){
    vkeys[tkeys(i)]=0;
    tempKeys[tkeys(i)]=0;
  }
  
  for(int i=0;i<4;i++){
    vaxis[taxis(i)]=0;
  }
  
  selected[0]=selected[1]=0;
  keyRepeatFrequency = 10;
}


MXML::Tag Controller::toXML(){
  Tag ret("controller");
  ret.addChildren(Tag("id"));
  ret["id"].setAttrib(Attribute(getId()));
  for(int i=0;i<K_END;i++){
    Tag k("key");
    k.addChildren(Tag("name"));
    k.addChildren(Tag("state"));
    k["name"].setAttrib(Attribute(i));
    k["state"].setAttrib(Attribute(getKeys()[i]));
    ret.addChildren(k);
  }
  for(int i=0;i<A_END;i++){
    Tag a("axis");
    a.addChildren(Tag("name"));
    a.addChildren(Tag("value"));
    a["name"].setAttrib(Attribute(i));
    a["value"].setAttrib(Attribute(getAxis()[i]));
    ret.addChildren(a);
  }
  Tag s("selected");
  s.addChildren(Tag("i"));
  s.addChildren(Tag("j"));
  s["i"].setAttrib(Attribute((int)selected[0]));
  s["j"].setAttrib(Attribute((int)selected[1]));
  ret.addChildren(s);
  return ret;
}
#include <iostream>
using namespace std;
void Controller::fromXML(MXML::Tag &code){
  for(Tag t:code.getChildren()){
    if(t.getName().compare("key")==0){
      setKey(tkeys(t["name"].getAttrib().getInt()),t["state"].getAttrib().getInt());
    }
    if(t.getName().compare("axis")==0){
      setAxis(taxis(t["name"].getAttrib().getInt()),t["value"].getAttrib().getFloat());
    }
    if(t.getName().compare("selected")==0){
      selected[0]=t["i"].getAttrib().getInt();
      selected[1]=t["j"].getAttrib().getInt();            
    }
  }
}

MXML::Tag Controller::difference(Networkable &n){
  Tag ret;
  try{
    Controller &c = dynamic_cast<Controller&> (n);
  }catch (bad_cast& bc){
    throw("bad_cast in controller::difference");
  }
  return ret;
}

const bool* Controller::getKeys(){
    return vkeys;
}

bool Controller::isKeyPressed(tkeys key)
{
  return vkeys[key] && !lastPressed[key];
}

const float* Controller::getAxis(){
  return vaxis;
}

const unsigned int* Controller::getSelected(){
  return selected;
}

void Controller::setKey(tkeys key,bool value){
  tempKeys[key]=value;
  if(value){
    vkeys[key]=value;
  }
}

void Controller::setAxis(taxis axis,float value){
  tempAxis[axis] = value;
  if(fabs(tempAxis[axis]) > fabs(vaxis[axis])){
    vaxis[axis]=value;
  }
}

void Controller::setSelection(unsigned int* sel){
  selected[0] = sel[0];
  selected[1] = sel[1];
}

int Controller::getId(){
  return id;
}

void Controller::finishIteration()
{
  for(int i=0; i < K_END; i++){
    if(vkeys[i]){
      lastPressed[i] = (lastPressed[i] + 1) % keyRepeatFrequency;
    }else{
      lastPressed[i] = 0;
    }
  }
  //if information is not received from controller, axes must be 0
  for(float &f:vaxis){
    f=0;
  }
  for(int i=0; i < K_END; i++){
    vkeys[i] = tempKeys[i];
  }
}

void Controller::init(lua_State* L){
    add_enum_to_lua(L,"KEYS","UP",K_UP,"DOWN",K_DOWN,"LEFT",
            K_LEFT,"RIGHT",K_RIGHT,"A",K_A,"B",K_B,"X",K_X,"Y",K_Y,"Z",K_Z,"R1",
            K_R1,"R2",K_R2,"L1",K_L1,"L2",K_L2,"START",K_START,NULL);
    add_enum_to_lua(L,"AXIS","X",A_X,"Y",A_Y,"R",A_R,"L",A_L,NULL);
}
