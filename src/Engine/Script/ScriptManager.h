#ifndef _SCRIPT_MANAGER_H_
#define _SCRIPT_MANAGER_H_

#include <vector>
#include <ObjectTypes/Scripted.h>

class Scripted;
class State;
struct lua_State;

class ScriptManager
{
public:
  ScriptManager(State* state);
  void registerElement(Scripted* scripted, string script);
  void update();
private:
  Scripted* currObject;
  int lastInsertedRef;
  static ScriptManager* active;
  State* state;
  int loadScript(string name, string dir, int reference);
  lua_State* L;
  std::map<int, Scripted*> list;
  
    
  static int LaddToScene(lua_State *L);

  static int LloadObject(lua_State *L);
  
  static int Lload(lua_State *L);
  
  static int LlockOrientation(lua_State *L);
  
  static int LsetPos(lua_State *L);
  static int LgetPos(lua_State *L);
  static int LsetRot(lua_State *L);
  static int LgetRot(lua_State *L);  
  static int LaddRot(lua_State *L);
  static int LsetSize(lua_State *L);
  static int LgetSize(lua_State *L);
  
  static int LgetName(lua_State *L);
  
  static int LsetLinVel(lua_State *L);
  static int LgetLinVel(lua_State *L);
  static int LsetAngVel(lua_State *L);
  static int LgetAngVel(lua_State *L);
  
  static int LgetKey(lua_State *L);
  static int LgetAxis(lua_State *L);
  
  static int LnewCam(lua_State *L);
  static int LchangeCurrCam(lua_State *L);
  static int LgetCamPos(lua_State *L);
  static int LsetCamPos(lua_State *L);
  static int LsetCamTarget(lua_State *L);
  
  static int LgetCollidingObjects(lua_State *L);
  
  static int LdeleteObject(lua_State *L);
  
  static int LgetObjectAnimations(lua_State *L);
  static int LgetCurrentObjectAnimation(lua_State *L);
  static int LchangeObjectAnimation(lua_State *L);
  static int LgetAnimationTime(lua_State *L);
  static int LsetLoopAnimation(lua_State *L);
  
  static int LgetCurrentText(lua_State *L);
  static int LsetCurrentText(lua_State *L);
};

#endif
