/**
 * @(#) Scripted.cpp
 */

#include "Scripted.h"
#include "../Base.h"
#include "../../ProjectManagement/Project.h"
#include "../lua_typed_enums.h"
#include "Physical.h"


#ifdef ANDROID
#include <android/log.h>

#define  LOG_TAG    "libgl2jni"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#endif


lua_State* Scripted::L=NULL;
vector<Scripted*> Scripted::list;
Scripted* Scripted::currObject;

Scripted::Scripted(string file, Vector3 p, Vector3 s, Vector3 r):Tile(p,s,r){
    currObject = NULL;
    loadFromFile(file);
}

string Scripted::getPath()
{
  return Base::getInstance()->getProj()->getDir(name,Project::OBJECT)+string(".o3s");
}

string Scripted::getTemplate(string name)
{
  string t=name+" = {self.ref}\n"
	"\n"
	"function new"+name+"()\n"
	"	local o= "+name+":New()\n"
	"	return o\n"
	"end\n"
	"\n"
	"function "+name+":New()\n"
	"	local o = {}\n"
	"	setmetatable(o, self)\n"
	"	self.__index = self\n"
	"		self.ref=registerObject()\n"
	"	return o\n"
	"end\n"
	"\n"
	"function "+name+":Update()\n"
	"end\n";
  return t;
}

void Scripted::loadFromFile(string file){
    name =file;
    if(name.find('.')!=string::npos){
      if(name.substr(name.find_last_of('.'),3).compare("o3s")){
	name=name.substr(0,name.find_last_of('.'));
      }
    }
    if(!L)Init();
    
    lua_getglobal(L,(string("new")+name).c_str());
    if (lua_isnil(L,-1) == 1){
        if(luaL_dofile(L, getPath().c_str())){
            cout << "Error reading file " << getPath() << endl;
            return;
        }
        
        lua_getglobal(L,(string("new")+name).c_str());
        if (lua_isnil(L,-1) == 1){
            cout << "Uninitialised function new" << name << endl;
            return;
        }
    }
    list.push_back(this);
    if(lua_pcall(L,0,1,0)){
        cerr << "Error loading " << name << " in lua:" << endl;
        cerr << "\t" << lua_tostring(L, -1) << endl;
        lua_pop(L, 1);
    }
    
    luaRef = luaL_ref(L,LUA_REGISTRYINDEX);
}

void Scripted::update( )
{
    currObject=this;
    lua_rawgeti(L, LUA_REGISTRYINDEX, luaRef);//object
    lua_getfield(L,-1,"Update");//function name
    lua_pushvalue(L,-2);//self
    if(lua_pcall(L,1,1,0)){
        cerr << "Error updating " << name << " in lua:" << endl;
        cerr << "\t" << lua_tostring(L, -1) << endl;
        lua_pop(L, 1);
    }
}

string Scripted::getName()
{
  return name;
}

void Scripted::Init(){    
    L = luaL_newstate();
    luaL_openlibs(L);
    
    lua_register(L,"addToScene",LaddToScene);
    lua_register(L, "load", Lload);
    lua_register(L, "registerObject",Scripted::LregisterObject);
    lua_register(L, "loadObject",Scripted::LloadObject);
    lua_register(L, "getObject",Scripted::LgetObject);
    lua_register(L, "lockOrientation",Scripted::LlockOrientation);
    lua_register(L, "getLinVel", Scripted::LgetLinVel);
    lua_register(L, "setLinVel", Scripted::LsetLinVel);
    lua_register(L, "getAngVel", Scripted::LgetAngVel);
    lua_register(L, "setAngVel", Scripted::LsetAngVel);
    lua_register(L, "getKey", Scripted::LgetKey);
    lua_register(L, "getAxis", Scripted::LgetAxis);
    lua_register(L, "newCam", Scripted::LnewCam);
    lua_register(L, "changeCurrCam", Scripted::LchangeCurrCam);
    lua_register(L, "getCamPos", Scripted::LgetCamPos);
    lua_register(L, "setCamPos", Scripted::LsetCamPos);
    lua_register(L, "setCamTarget", Scripted::LsetCamTarget);
    lua_register(L, "setPos", Scripted::LsetPos);
    lua_register(L, "getPos", Scripted::LgetPos);
    lua_register(L, "setRot", Scripted::LsetRot);
    lua_register(L, "getRot", Scripted::LgetRot);
    lua_register(L, "addRot", Scripted::LaddRot);
    lua_register(L, "aAtoQuaternion", Scripted::LAAtoQuaternion);
    Controller::init(L);
}

Scripted* Scripted::getByInd(int ind){
    return list.at(ind);
}

edType Scripted::getType(){
    return E_OBJECT;
}

#include "Tile.h"
#include "Camera.h"
class Tile;

int Scripted::LaddToScene(lua_State* L){
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
    
    Base::getInstance()->getCurState()->addElement(new Tile(pos,siz,rot,dir));
    return 0;
}

int Scripted::Lload(lua_State *L){
    int n= lua_gettop(L);
    if(n<1){
        lua_pushstring(L,"Error in lua load");
        lua_error(L);
    }
    
    int ind = lua_tointeger(L,1);
    string dir=lua_tostring(L,2);
    Vector3 pos, siz(1,1,1);
    Quaternion rot(0,0,0,1);
    Scripted * tmp=getByInd(ind);
    
    tmp->setResource(dir);
    
    if(n>3){
        pos.x = lua_tonumber(L,3);
        pos.y = lua_tonumber(L,4);
        pos.z = lua_tonumber(L,5);
	tmp->position=pos;
    }
    
    if(n>6){
        rot.x = lua_tonumber(L,6);
        rot.y = lua_tonumber(L,7);
        rot.z = lua_tonumber(L,8);
        rot.w = lua_tonumber(L,9);
	tmp->rotation=rot;
    }
    
    if(n>10){
        siz.x = lua_tonumber(L,10);
        siz.y = lua_tonumber(L,11);
        siz.z = lua_tonumber(L,12);
	tmp->size=siz;
    }
    lua_pushnumber(L,0);
    return 1;
}

int Scripted::LregisterObject(lua_State *L){    
    int n= lua_gettop(L);
    if(n!=0){
        lua_pushstring(L,"Error in lua registerNewObject");
        lua_error(L);
    }
    
    lua_pushnumber(L,list.size()-1);
    return 1;
}

int lastRef=-1;
string lastname="";

int Scripted::LloadObject(lua_State *L){
    int n= lua_gettop(L);
    if(n!=1){
        lua_pushstring(L,"Error in lua loadObject");
        lua_error(L);
    }
    
    string file=lua_tostring(L,1);
    
    Scripted *tmp= new Scripted(file);
    Base::getInstance()->getCurState()->addElement(tmp);
    list.push_back(tmp);
    
    lastname=file;
    lastRef=tmp->luaRef;
    return 0;
}

int Scripted::LgetObject(lua_State *L){
    
    lua_rawgeti(L, LUA_REGISTRYINDEX, lastRef);
    luaL_checktype(L, -1, LUA_TTABLE); 
    
    
    luaL_getmetatable(L,lastname.c_str());
    lua_setmetatable(L,-1);
    
    return 1;
}

int Scripted::LlockOrientation(lua_State* L){
    int n= lua_gettop(L);
    if(n>1){
        lua_pushstring(L,"Error in lua lock orientation");
        lua_error(L);
    }
    
    Scripted* current;
    
    if(n>0){
        int ind = lua_tointeger(L,1);
        current = getByInd(ind);
    }else{
        current = currObject;
    }
    
    if(current&&current->getPhysical()){
        current->getPhysical()->setSleepingThresholds(0.0, 0.0);
        current->getPhysical()->setAngularFactor(0.0);
    }
    return 0;
}

int Scripted::LgetPos(lua_State* L){
    int n= lua_gettop(L);
    if(n>1){
        lua_pushstring(L,"Error in lua getPos");
        lua_error(L);
    }
    Vector3 ret;
    if(n>0){
        int ind = lua_tointeger(L,1);
        ret = getByInd(ind)->getPos();
    }else{
        ret = currObject->getPos();
    }
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);    
    
    return 3;
}

int Scripted::LsetPos(lua_State* L){
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
        getByInd(ind)->setPos(pos);
    }else{
        currObject->setPos(pos);
    }
    
    return 0;
}

int Scripted::LgetRot(lua_State* L){
    int n= lua_gettop(L);
    if(n>1){
        lua_pushstring(L,"Error in lua getRot");
        lua_error(L);
    }
    Quaternion ret;
    if(n>0){
        int ind = lua_tointeger(L,1);
        ret = getByInd(ind)->getRot();
    }else{
        ret = currObject->getRot();
    }
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);    
    lua_pushnumber(L,ret.w);
    return 4;
}

int Scripted::LsetRot(lua_State* L){
    int n= lua_gettop(L);
    if(n!=4&&n!=5){
        lua_pushstring(L,"Error in lua setRot");
        lua_error(L);
    }
    Quaternion rot;
    rot.x=lua_tonumber(L,1);
    rot.y=lua_tonumber(L,2);
    rot.z=lua_tonumber(L,3);
    rot.w=lua_tonumber(L,4);
    Scripted* obj;
    if(n>4){
        int ind = lua_tointeger(L,5);
        obj = getByInd(ind);
    }else{
        obj= currObject;
    }
    if(obj){
        obj->setRot(rot);
    }
    
    return 0;
}

int Scripted::LaddRot(lua_State* L){
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
    Scripted* obj;
    if(n>4){
        int ind = lua_tointeger(L,5);
        obj = getByInd(ind);
    }else{
        obj= currObject;
    }
    if(obj){
        obj->setRot(obj->getRot()*rot);
    }
    
    return 0;
}

int Scripted::LgetSize(lua_State* L){
    int n= lua_gettop(L);
    if(n>1){
        lua_pushstring(L,"Error in lua getSize");
        lua_error(L);
    }
    Vector3 ret;
    if(n>0){
        int ind = lua_tointeger(L,1);
        ret = getByInd(ind)->getSize();
    }else{
        ret = currObject->getSize();
    }
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);    
    
    return 3;
}

int Scripted::LsetSize(lua_State* L){
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
        getByInd(ind)->setSize(size);
    }else{
        currObject->setSize(size);
    }
    
    return 0;
}

int Scripted::LsetLinVel(lua_State *L){
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
        getByInd(ind)->setLinVel(Vector3(x,y,z));
    }else
        currObject->setLinVel(Vector3(x,y,z));
    return 0;    
}

int Scripted::LsetAngVel(lua_State *L){    
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
        getByInd(ind)->setAngVel(Vector3(x,y,z));
    }else{
        currObject->setAngVel(Vector3(x,y,z));
    }
    return 0; 
}

int Scripted::LgetLinVel(lua_State *L){
    int n= lua_gettop(L);
    Vector3 ret;
    if(n>1){
        lua_pushstring(L,"Error in lua get lin vel");
        lua_error(L);
    }
    if(n>0){
        int ind = lua_tointeger(L,1);
        ret = getByInd(ind)->getLinVel();
    }else{
        ret = currObject->getLinVel();
    }
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);    
    return 3;
}

int Scripted::LgetAngVel(lua_State *L){
    int n= lua_gettop(L);
    Vector3 ret;
    if(n>1){
        lua_pushstring(L,"Error in lua get ang vel");
        lua_error(L);
    }
    if(n>0){
        int ind = lua_tointeger(L,1);
        ret = getByInd(ind)->getAngVel();
    }else{
        ret = currObject->getAngVel();
    }
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);
    return 3;
}

int Scripted::LgetKey(lua_State *L){
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

int Scripted::LgetAxis(lua_State *L){
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

int Scripted::LnewCam(lua_State* L){
    int n= lua_gettop(L);
    if(n!=0){
        lua_pushstring(L,"Error in lua new cam");
        lua_error(L);
    }
    int ret= Base::getInstance()->getCurState()->addCam();
    lua_pushnumber(L,ret);
    return 1;
}

int Scripted::LchangeCurrCam(lua_State *L){
    int n= lua_gettop(L);
    if(n!=1){
        lua_pushstring(L,"Error in lua change cur cam");
        lua_error(L);
    }
    
    int newCam = lua_tointeger(L,1);
    Base::getInstance()->getCurState()->setCam(newCam);
    return 0;
}

int Scripted::LgetCamPos(lua_State *L){
    int n= lua_gettop(L);
    if(n!=0){
        lua_pushstring(L,"Error in lua getcampos");
        lua_error(L);
    }
    
    Vector3 ret=Base::getInstance()->getCurState()->getCam()->getPos();
    lua_pushnumber(L,ret.x);
    lua_pushnumber(L,ret.y);
    lua_pushnumber(L,ret.z);
    return 3;
}

int Scripted::LsetCamPos(lua_State *L){
    int n= lua_gettop(L);
    if(n!=3){
        lua_pushstring(L,"Error in lua setcampos");
        lua_error(L);
    }
    Vector3 pos;
    pos.x= lua_tonumber(L,1);
    pos.y= lua_tonumber(L,2);
    pos.z= lua_tonumber(L,3);
    
    Base::getInstance()->getCurState()->getCam()->setPos(pos);
    
    return 0;
}

int Scripted::LsetCamTarget(lua_State *L){
    int n= lua_gettop(L);
    if(n!=3){
        lua_pushstring(L,"Error in lua setcamTarget");
        lua_error(L);
    }
    Vector3 pos;
    pos.x= lua_tonumber(L,1);
    pos.y= lua_tonumber(L,2);
    pos.z= lua_tonumber(L,3);
    Base::getInstance()->getCurState()->getCam()->setTarget(pos);
    
    return 0;
}

int Scripted::LAAtoQuaternion(lua_State *L){
    int n=lua_gettop(L);
    if(n!=4){
        lua_pushstring(L,"Error in angle axis to quaternion");
        lua_error(L);
    }
    Quaternion q= Quaternion::fromAngleMagnitude(lua_tonumber(L,1), lua_tonumber(L,2), 
            lua_tonumber(L,3), lua_tonumber(L,4));
    
    lua_pushnumber(L,q.w);
    lua_pushnumber(L,q.x);
    lua_pushnumber(L,q.y);
    lua_pushnumber(L,q.z);
    
    return 4;
}

using namespace MXML;


Scripted::Scripted(MXML::Tag &code):Tile(Vector3::zero,Vector3(1,1,1),Vector3::zero){
    fromXML(code);
}

void Scripted::fromXML( MXML::Tag &code ){
    if(code.contains("file"))loadFromFile(code["file"].getAttrib().getString());
    if(code.contains("tile")){
      Tile::fromXML(code["tile"]);
    }
}

MXML::Tag Scripted::toXML( ){
    Tag ret("scripted");
    ret.addChildren(Tag("id"));
    ret["id"].setAttrib(Attribute(int(id)));
    ret.addChildren(Tag("file"));
    ret.addChildren(Tile::toXML());
    ret["file"].setAttrib(Attribute(string(name).c_str()));
    return ret;
}
