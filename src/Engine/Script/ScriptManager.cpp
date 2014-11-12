#include <string>
#include <iostream>

#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>

#include "Base.h"
#include "ScriptManager.h"
#include "Controller.h"
#include "ObjectTypes/Scripted.h"
#include "States/State.h"
#include "lua_typed_enums.h"

ScriptManager* ScriptManager::active = nullptr;

ScriptManager::ScriptManager(State* state)
  :state(state)
{
  L = luaL_newstate();
  luaL_openlibs(L);
  lastInsertedRef = -1;
  
  lua_register(L,"addToScene",LaddToScene);
  lua_register(L, "load", Lload);
  lua_register(L, "loadObject",ScriptManager::LloadObject);
  lua_register(L, "lockOrientation",ScriptManager::LlockOrientation);
  lua_register(L, "getLinVel", ScriptManager::LgetLinVel);
  lua_register(L, "setLinVel", ScriptManager::LsetLinVel);
  lua_register(L, "getAngVel", ScriptManager::LgetAngVel);
  lua_register(L, "setAngVel", ScriptManager::LsetAngVel);
  lua_register(L, "getKey", ScriptManager::LgetKey);
  lua_register(L, "getAxis", ScriptManager::LgetAxis);
  lua_register(L, "newCam", ScriptManager::LnewCam);
  lua_register(L, "changeCurrCam", ScriptManager::LchangeCurrCam);
  lua_register(L, "getCamPos", ScriptManager::LgetCamPos);
  lua_register(L, "setCamPos", ScriptManager::LsetCamPos);
  lua_register(L, "setCamTarget", ScriptManager::LsetCamTarget);
  lua_register(L, "setPos", ScriptManager::LsetPos);
  lua_register(L, "getPos", ScriptManager::LgetPos);
  lua_register(L, "setRot", ScriptManager::LsetRot);
  lua_register(L, "getRot", ScriptManager::LgetRot);
  lua_register(L, "addRot", ScriptManager::LaddRot);
  lua_register(L, "setSize", ScriptManager::LsetSize);
  lua_register(L, "getSize", ScriptManager::LgetSize);
  lua_register(L, "getObjectAnimations",ScriptManager::LgetObjectAnimations);
  lua_register(L, "getCurrentAnimation",ScriptManager::LgetCurrentObjectAnimation);
  lua_register(L, "changeObjectAnimation",ScriptManager::LchangeObjectAnimation);
  lua_register(L, "getAnimationTime", ScriptManager::LgetAnimationTime);
  lua_register(L, "setLoopAnimation", ScriptManager::LsetLoopAnimation);
  lua_register(L,"getName",ScriptManager::LgetName);
  lua_register(L,"getCollidingObjects", ScriptManager::LgetCollidingObjects);
  lua_register(L,"deleteObject",ScriptManager::LdeleteObject);
  Controller::init(L); 
}

void ScriptManager::registerElement(Scripted* scripted, string script)
{
  active = this;
  lastInsertedRef++;
  list.insert(std::pair<int, Scripted*>(lastInsertedRef, scripted));
  scripted->luaRef = loadScript(script, scripted->getPath(), lastInsertedRef);
  cout << "Set luaref to " << scripted->luaRef << " in " << script << std::endl;
  scripted->name = script.substr(0,script.find_last_of('.'));
}

void ScriptManager::update()
{
  std::map<int, Scripted*>::iterator it;
  bool needsDelete = false;
  it = list.begin();
  while (it != list.end())
  {
    currObject = it->second;

    lua_rawgeti(L, LUA_REGISTRYINDEX, it->second->luaRef);//object
    lua_getfield(L,-1,"Update");//function name
    lua_pushvalue(L,-2);//self
    //Allways at the end, in case the caller is being deleted!!!
    if(lua_pcall(L,1,1,0)){
        cerr << "Error updating " << it->second->name << " in lua:" << endl;
        cerr << "\t" << lua_tostring(L, -1) << endl;
        lua_pop(L, 1);
    }
    if (currObject->needsDelete())
    {
      needsDelete = true;
    }
    ++it;
  }
  if (needsDelete)
  {
    it = list.begin();
    while (it != list.end())
    {
      currObject = it->second;
      if (currObject->needsDelete())
      {
	state->deleteElement(currObject);
	it = list.erase(it);
      }
      else{
	++it;
      }
    }
  }
}

int ScriptManager::loadScript(string name, string dir, int reference)
{
  int ret = -1;
  cout << "Name is " << name << " at " << this << endl;
  lua_getglobal(L,(string("new")+name).c_str());
  if (lua_isnil(L,-1) == 1){
    cout << "Need to load file from " << dir << std::endl;
    if(luaL_dofile(L, dir.c_str())){
      std::cout << "Error reading file " << dir << std::endl;
      return -1;
    }
    cout << "File loaded\n";
    lua_getglobal(L,(string("new")+name).c_str());
    cout << "Got global " << string("new")+name << std::endl;
  }
  if (lua_isnil(L,-1) == 1){
    std::cout << "Uninitialised function new" << name << std::endl;
  }
  else
  {
    cout << "calling new with reference " << reference << std::endl;
    lua_pushnumber(L, reference);
    if(lua_pcall(L,1,1,0)){
      std::cerr << "Error loading " << name << " in lua:" << std::endl;
      std::cerr << "\t" << lua_tostring(L, -1) << std::endl;
      lua_pop(L, 1);
    }
    else
    {
      ret = luaL_ref(L,LUA_REGISTRYINDEX);
    }
  }
  return ret;
}


#include "ObjectTypes/Tile.h"
#include "ObjectTypes/Camera.h"
#include "Resources/Model3d.h"
#include "Resources/Text.h"

int ScriptManager::LaddToScene(lua_State* L){
    int n=lua_gettop(L);
    if(n<1){
        lua_pushstring(L,"Error in lua add to scene");
        lua_error(L);
    }
    string dir=lua_tostring(L,1);
    Vector3 pos, siz(1,1,1);
    Quaternion rot(0,0,0,1);
    if(n>3){
        pos.x = lua_tonumber(L,2);
        pos.y = lua_tonumber(L,3);
        pos.z = lua_tonumber(L,4);
    }
    
    if(n>6){
        rot.x = lua_tonumber(L,5);
        rot.y = lua_tonumber(L,6);
        rot.z = lua_tonumber(L,7);
        rot.w = lua_tonumber(L,8);
    }
    
    if(n>10){
        siz.x = lua_tonumber(L,9);
        siz.y = lua_tonumber(L,10);
        siz.z = lua_tonumber(L,11);
    }
    
    active->state->addElement(new Tile(active->state, pos, siz, rot, dir));
    return 0;
}

int ScriptManager::Lload(lua_State *L){
    int n= lua_gettop(L);
    if(n<1){
        lua_pushstring(L,"Error in lua load");
        lua_error(L);
    }
    
    int ind = lua_tointeger(L,1);
    string dir;
    if(n>1){
      dir=lua_tostring(L,2);
    }
    Vector3 pos, siz(1,1,1);
    Quaternion rot(0,0,0,1);
    Scripted * tmp=active->list.find(ind)->second;
    if(tmp)
    {
      if(!dir.empty()){
	tmp->setResource(dir);
      }
      
      if(n>3){
	  pos.x = lua_tonumber(L,3);
	  pos.y = lua_tonumber(L,4);
	  pos.z = lua_tonumber(L,5);
	  tmp->setPos(pos);
      }
      
      if(n>6){
	  rot.x = lua_tonumber(L,6);
	  rot.y = lua_tonumber(L,7);
	  rot.z = lua_tonumber(L,8);
	  rot.w = lua_tonumber(L,9);
	  tmp->setRot(rot);
      }
      
      if(n>10){
	  siz.x = lua_tonumber(L,10);
	  siz.y = lua_tonumber(L,11);
	  siz.z = lua_tonumber(L,12);
	  tmp->setSize(siz);
      }
      lua_pushnumber(L,0);
    }
    return 1;
}

int ScriptManager::LloadObject(lua_State *L){
    int n= lua_gettop(L);
    if(n!=1){
        lua_pushstring(L,"Error in lua loadObject");
        lua_error(L);
    }
    string file=lua_tostring(L,1);
    
    Scripted *tmp= new Scripted(active->state, file, active);
    active->state->addElement(tmp);
    lua_rawgeti(L, LUA_REGISTRYINDEX, tmp->luaRef);
    luaL_checktype(L, -1, LUA_TTABLE); 

    luaL_getmetatable(L, file.c_str());
    lua_setmetatable(L,-1);
    return 1;
}

int ScriptManager::LlockOrientation(lua_State* L){
    int n= lua_gettop(L);
    if(n>1){
        lua_pushstring(L,"Error in lua lock orientation");
        lua_error(L);
    }
    
    Scripted* current;
    
    if(n>0){
        int ind = lua_tointeger(L,1);
        current = active->list.find(ind)->second;
    }else{
        current = active->currObject;
    }
    
    if(current&&current->getPhysical()){
        current->getPhysical()->setSleepingThresholds(0.0, 0.0);
        current->getPhysical()->setAngularFactor(0.0);
    }
    return 0;
}

int ScriptManager::LgetPos(lua_State* L){
    int n= lua_gettop(L);
    if(n>1){
        lua_pushstring(L,"Error in lua getPos");
        lua_error(L);
    }
    Vector3 ret;
    if(n>0){
        int ind = lua_tointeger(L,1);
        ret = active->list.find(ind)->second->getPos();
    }else{
        ret = active->currObject->getPos();
    }
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);    
    
    return 3;
}

int ScriptManager::LsetPos(lua_State* L){
    int n= lua_gettop(L);
    if(n!=3&&n!=4){
        lua_pushstring(L,"Error in lua getPos");
        lua_error(L);
    }
    Vector3 pos;
    pos.x=lua_tonumber(L,1);
    pos.y=lua_tonumber(L,2);
    pos.z=lua_tonumber(L,3);
    if(n>3){
        int ind = lua_tointeger(L,4);
        active->list.find(ind)->second->setPos(pos);
    }else{
        active->currObject->setPos(pos);
    }
    
    return 0;
}

int ScriptManager::LgetRot(lua_State* L){
    int n= lua_gettop(L);
    if(n>1){
        lua_pushstring(L,"Error in lua getRot");
        lua_error(L);
    }
    Quaternion q;
    if(n>0){
        int ind = lua_tointeger(L,1);
        q = active->list.find(ind)->second->getRot();
    }else{
        q = active->currObject->getRot();
    }
    Vector3 ret=q.toEuler();
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);
    return 3;
}

int ScriptManager::LsetRot(lua_State* L){
    int n= lua_gettop(L);
    if(n!=3&&n!=4){
        lua_pushstring(L,"Error in lua setRot");
        lua_error(L);
    }
    Vector3 rot;
    rot.x=lua_tonumber(L,1);
    rot.y=lua_tonumber(L,2);
    rot.z=lua_tonumber(L,3);
    Quaternion q(rot);
    Scripted* obj;
    if(n>3){
        int ind = lua_tointeger(L,4);
        obj = active->list.find(ind)->second;
    }else{
        obj= active->currObject;
    }
    if(obj){
        obj->setRot(q);
    }
    
    return 0;
}

int ScriptManager::LaddRot(lua_State* L){
    int n= lua_gettop(L);
    if(n!=4&&n!=5){
        lua_pushstring(L,"Error in lua addRot");
        lua_error(L);
    }
    Quaternion rot;
    rot.x=lua_tonumber(L,1);
    rot.y=lua_tonumber(L,2);
    rot.z=lua_tonumber(L,3);
    rot.w=lua_tonumber(L,4);
    rot.fromAngleMagnitude(rot.x,rot.y,rot.z,rot.w);
    Scripted* obj;
    if(n>4){
        int ind = lua_tointeger(L,5);
        obj = active->list.find(ind)->second;
    }else{
        obj= active->currObject;
    }
    if(obj){
        obj->setRot(obj->getRot()*rot);
    }
    
    return 0;
}

int ScriptManager::LgetSize(lua_State* L){
    int n= lua_gettop(L);
    if(n>1){
        lua_pushstring(L,"Error in lua getSize");
        lua_error(L);
    }
    Vector3 ret;
    if(n>0){
        int ind = lua_tointeger(L,1);
        ret = active->list.find(ind)->second->getSize();
    }else{
        ret = active->currObject->getSize();
    }
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);    
    
    return 3;
}

int ScriptManager::LsetSize(lua_State* L){
    int n= lua_gettop(L);
    if(n!=3&&n!=4){
        lua_pushstring(L,"Error in lua setSize");
        lua_error(L);
    }
    Vector3 size;
    size.x=lua_tonumber(L,1);
    size.y=lua_tonumber(L,2);
    size.z=lua_tonumber(L,3);
    if(n>3){
        int ind = lua_tointeger(L,4);
        active->list.find(ind)->second->setSize(size);
    }else{
        active->currObject->setSize(size);
    }
    
    return 0;
}

int ScriptManager::LgetName(lua_State* L)
{
  int n=lua_gettop(L);
  Tile* current=active->currObject;
  if(n>0){
    int ind = lua_tonumber(L,1);
    cout << "Getting name " << ind << std::endl;
    current=active->list.find(ind)->second;
  }
  
  string name;
  
  if(current->getType()==E_OBJECT){
    Scripted* object = dynamic_cast<Scripted*>(current);
    name=object->getName();
    cout << "= " << name << std::endl;
  }else{
    name= current->getResource()->getName();
  }

  lua_pushstring(L,name.c_str());
  return 1;
}

int ScriptManager::LsetLinVel(lua_State *L){
    int n= lua_gettop(L);
    if(n<2||n>4){
        lua_pushstring(L,"Error in lua set lin vel");
        lua_error(L);
    }
    float x = lua_tonumber(L,1);
    float y = lua_tonumber(L,2);
    float z =0;
    int ind=0;
    if(n>2)
        z = lua_tonumber(L,3);
    if(n>3){
        ind = lua_tonumber(L,4);
        active->list.find(ind)->second->setLinVel(Vector3(x,y,z));
    }else
        active->currObject->setLinVel(Vector3(x,y,z));
    return 0;    
}

int ScriptManager::LsetAngVel(lua_State *L){    
    int n= lua_gettop(L);
    if(n<2||n>4){
        lua_pushstring(L,"Error in lua setAngVel");
        lua_error(L);
    }
    int ind = 0;
    float x = lua_tonumber(L,1);
    float y = lua_tonumber(L,2);
    float z =0;
    if(n>2){        
        z = lua_tonumber(L,3);
    }
    if(n>3){
        ind = lua_tonumber(L,4);
        active->list.find(ind)->second->setAngVel(Vector3(x,y,z));
    }else{
        active->currObject->setAngVel(Vector3(x,y,z));
    }
    return 0; 
}

int ScriptManager::LgetLinVel(lua_State *L){
    int n= lua_gettop(L);
    Vector3 ret;
    if(n>1){
        lua_pushstring(L,"Error in lua get lin vel");
        lua_error(L);
    }
    if(n>0){
        int ind = lua_tointeger(L,1);
        ret = active->list.find(ind)->second->getLinVel();
    }else{
        ret = active->currObject->getLinVel();
    }
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);    
    return 3;
}

int ScriptManager::LgetAngVel(lua_State *L){
    int n= lua_gettop(L);
    Vector3 ret;
    if(n>1){
        lua_pushstring(L,"Error in lua get ang vel");
        lua_error(L);
    }
    if(n>0){
        int ind = lua_tointeger(L,1);
        ret = active->list.find(ind)->second->getAngVel();
    }else{
        ret = active->currObject->getAngVel();
    }
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);
    return 3;
}

int ScriptManager::LgetKey(lua_State *L){
    int n= lua_gettop(L);
    if(n!=2){
        lua_pushstring(L,"Error in lua is pressed");
        lua_error(L);
    }
    int no = lua_tointeger(L,1);
    if(!Base::getInstance()->getControllers().empty())
        lua_pushboolean(L,Base::getInstance()->getControllers()[0]->getKeys()[get_enum_value(L, 2)]);
    return 1;
}

int ScriptManager::LgetAxis(lua_State *L){
    int n= lua_gettop(L);
    if(n!=2){
        lua_pushstring(L,"Error in lua get axis");
        lua_error(L);
    }
    int no = lua_tointeger(L,1);
    if(!Base::getInstance()->getControllers().empty())
        lua_pushnumber(L,Base::getInstance()->getControllers()[no]->getAxis()[get_enum_value(L, 2)]);
    return 1;
}

int ScriptManager::LnewCam(lua_State* L){
    int n= lua_gettop(L);
    if(n!=0){
        lua_pushstring(L,"Error in lua new cam");
        lua_error(L);
    }
    int ret= active->state->addCam();
    lua_pushnumber(L,ret);
    return 1;
}

int ScriptManager::LchangeCurrCam(lua_State *L){
    int n= lua_gettop(L);
    if(n!=1){
        lua_pushstring(L,"Error in lua change cur cam");
        lua_error(L);
    }
    
    int newCam = lua_tointeger(L,1);
    active->state->setCam(newCam);
    return 0;
}

int ScriptManager::LgetCamPos(lua_State *L){
    int n= lua_gettop(L);
    if(n!=0){
        lua_pushstring(L,"Error in lua getcampos");
        lua_error(L);
    }
    
    Vector3 ret=active->state->getCam()->getPos();
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);
    return 3;
}

int ScriptManager::LsetCamPos(lua_State *L){
    int n= lua_gettop(L);
    if(n!=3){
        lua_pushstring(L,"Error in lua setcampos");
        lua_error(L);
    }
    Vector3 pos;
    pos.x= lua_tonumber(L,1);
    pos.y= lua_tonumber(L,2);
    pos.z= lua_tonumber(L,3);
    
    active->state->getCam()->setPos(pos);
    
    return 0;
}

int ScriptManager::LsetCamTarget(lua_State *L){
    int n= lua_gettop(L);
    if(n!=3){
        lua_pushstring(L,"Error in lua setcamTarget");
        lua_error(L);
    }
    Vector3 pos;
    pos.x= lua_tonumber(L,1);
    pos.y= lua_tonumber(L,2);
    pos.z= lua_tonumber(L,3);
    active->state->getCam()->setTarget(pos);
    
    return 0;
}

int ScriptManager::LgetCollidingObjects(lua_State* L)
{
  int n=lua_gettop(L);
  Scripted* collider;
  if(n==0){
    collider=active->currObject;
  }else{
    int ind = lua_tointeger(L,1);
    collider=active->list.find(ind)->second;
  }
  
  vector<Tile*> colliding=collider->getColliding();
  vector<Scripted*> collidingObjects;
  
  for(Tile* t:colliding){
    if(t->getType()==E_OBJECT){
      collidingObjects.push_back(dynamic_cast<Scripted*>(t));
    }
  }
  
  lua_newtable(L);

  for (int i = 0; i < collidingObjects.size(); i++) {
    lua_rawgeti(L, LUA_REGISTRYINDEX, collidingObjects[i]->luaRef);
    luaL_checktype(L, -1, LUA_TTABLE); 

    luaL_getmetatable(L,collidingObjects[i]->name.c_str());
    lua_setmetatable(L,-1);
    lua_rawseti(L, -2, i+1);
    cout << "Added " << collidingObjects[i]->luaRef << "," << collidingObjects[i]->name.c_str() << std::endl;
  }
  
  return 1;
}

int ScriptManager::LdeleteObject(lua_State* L)
{
  int n = lua_gettop(L);
  if(n!=1){
    lua_pushstring(L,"Error in delete object");
    lua_error(L);
  }
  cout << "deleting object " << lua_tonumber(L,1) << std::endl;
  Scripted* toDelete=active->list.find(lua_tonumber(L,1))->second;
  toDelete->deleted=true;
}

int ScriptManager::LgetObjectAnimations(lua_State *L){
  int n = lua_gettop(L);
  if(n!=1){
    lua_pushstring(L,"Error getObjectAnimations");
    lua_error(L);
  }
  
  Scripted* object=active->list.find(lua_tonumber(L,1))->second;
  Resource* res=object->getResource();
  if(res->getType()!=Resource::MODEL3D)
    return 0;
  
  Model3d* model=dynamic_cast<Model3d*>(res);
  vector<string> animationNames=model->getAnimations();
  
  lua_createtable(L, animationNames.size(), 0);
  for (int i=0;i<animationNames.size();i++) {
    lua_pushstring(L, animationNames[i].c_str());
    lua_rawseti (L, -2, i);
  }
  
  return 1;
}

int ScriptManager::LgetCurrentObjectAnimation(lua_State *L){  
  int n = lua_gettop(L);
  if(n!=1){
    lua_pushstring(L,"Error in getCurrentObjectAnimation");
    lua_error(L);
  }
  
  Scripted* object=active->list.find(lua_tonumber(L,1))->second;
  Resource* res=object->getResource();
  if(res->getType()!=Resource::MODEL3D)
    return 0;
  
  Model3d* model=dynamic_cast<Model3d*>(res);
  
  lua_pushstring(L,model->getCurrentAnimation().c_str());
  
  return 1;
}

int ScriptManager::LchangeObjectAnimation(lua_State *L){
  int n = lua_gettop(L);
  if(n!=2){
    lua_pushstring(L,"Error in changeObjectAnimation");
    lua_error(L);
  }
  Scripted* object=active->list.find(lua_tonumber(L,1))->second;
  Resource* res=object->getResource();
  if(res->getType()!=Resource::MODEL3D)
    return 0;
  
  Model3d* model=dynamic_cast<Model3d*>(res);
  
  string name=lua_tostring(L,2);
  model->changeAnimation(name);
  
  return 0;
}

int ScriptManager::LgetAnimationTime(lua_State* L)
{
  int n = lua_gettop(L);
  if(n!=1){
    lua_pushstring(L,"Error in getAnimationTime");
    lua_error(L);
  }
  
  Scripted* object=active->list.find(lua_tonumber(L,1))->second;
  Resource* res=object->getResource();
  if(res->getType()!=Resource::MODEL3D)
    return 0;
  
  Model3d* model=dynamic_cast<Model3d*>(res);
  
  lua_pushnumber(L,model->getAnimationTime());
  
  return 1;
}

int ScriptManager::LsetLoopAnimation(lua_State* L)
{
  int n = lua_gettop(L);
  if(n!=2){
    lua_pushstring(L,"Error in setLoopAnimation");
    lua_error(L);
  }
  
  Scripted* object=active->list.find(lua_tonumber(L,1))->second;
  Resource* res=object->getResource();
  if(res->getType()!=Resource::MODEL3D)
    return 0;
  
  Model3d* model=dynamic_cast<Model3d*>(res);
  bool isLoop= lua_toboolean(L,2);
  model->setLoop(isLoop);
  
  return 0;
}

int ScriptManager::LgetCurrentText(lua_State *L)
{
  int n = lua_gettop(L);
  if(n!=1){
    lua_pushstring(L,"Error in getCurrentText");
    lua_error(L);
  }
  
  string ret;
  Scripted* object=active->list.find(lua_tonumber(L,1))->second;
  Resource* res= object->getResource();
  if(res->getType()==Resource::TEXT){
    Text* t= dynamic_cast<Text*>(res);
    ret = t->getText();
  }
  
  lua_pushstring(L,ret.c_str());  
  return 1;
}

int ScriptManager::LsetCurrentText(lua_State *L)
{
  int n = lua_gettop(L);
  if(n!=2){
    lua_pushstring(L,"Error in setCurrentText");
    lua_error(L);
  }
  Scripted* object=active->list.find(lua_tonumber(L,1))->second;
  Resource* res=object->getResource();
  if(res->getType()==Resource::TEXT)
  {
    string text=lua_tostring(L,2);
    Text* t= dynamic_cast<Text*>(res);
    t->setText(text);
  }
  return 0;
}
