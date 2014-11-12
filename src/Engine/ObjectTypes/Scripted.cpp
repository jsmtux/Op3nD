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

Scripted::Scripted(State* _state, string file, ScriptManager* scriptManager, Vector3 p, Vector3 s, Vector3 r)
  :Tile(_state,p,s,r)
  , scriptMan(scriptManager)
  , name(file)
{
  scriptManager->registerElement(this, file);
}

Scripted::~Scripted()
{
  
}

edType Scripted::getType()
{
  return E_OBJECT;
}

string Scripted::getPath()
{
  return Base::getInstance()->getProj()->getDir(name,Project::OBJECT)+string(".o3s");
}

string Scripted::getTemplate(string name)
{
  string t=name+" = {self.ref}\n"
	"\n"
	"function new"+name+"(ref)\n"
	"	local o= "+name+":New(ref)\n"
	"	return o\n"
	"end\n"
	"\n"
	"function "+name+":New()\n"
	"	local o = {}\n"
	"	setmetatable(o, self)\n"
	"	self.__index = self\n"
	"		self.ref=ref\n"
	"	return o\n"
	"end\n"
	"\n"
	"function "+name+":Update()\n"
	"end\n";
  return t;
}

bool Scripted::needsDelete()
{
  return deleted;
}
string Scripted::getName()
{
  return name;
}

using namespace MXML;

Scripted::Scripted(State* state, MXML::Tag &code, ScriptManager* scriptManager)
  :Tile(state, Vector3::zero,Vector3(1,1,1),Vector3::zero)
  , scriptMan(scriptManager)
{
    fromXML(code, scriptManager);
}

void Scripted::fromXML( MXML::Tag &code, ScriptManager* scriptManager)
{
  scriptMan = scriptManager;
  name = code["file"].getAttrib().getString();
  scriptManager->registerElement(this, name);
  cout << "Element registered\n";
  if(code.contains("tile")){
    Tile::fromXML(code["tile"]);
  }
  cout << "Exited fromxml\n";
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