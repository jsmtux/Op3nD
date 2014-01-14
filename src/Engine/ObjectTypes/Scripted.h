/**
 * @(#) Scripted.h
 */

#ifndef SCRIPTED_H_H
#define SCRIPTED_H_H

#include <lua5.2/lua.hpp>

#include "../Math/Vector3.h"

#include "Tile.h"
#include <map>
#include <string>
class Scripted: public Tile
{	
public:
    void update( );
    Scripted(string file, Vector3 p=0, Vector3 s=Vector3(1,1,1), Vector3 r=0);
    Scripted(MXML::Tag &code);
    ~Scripted();
    virtual void fromXML( MXML::Tag &code );
    virtual MXML::Tag toXML( );
    static lua_State *getState(){return L;}
    static void Init();
    edType getType();
    string getName();
    string getPath();
    static string getTemplate(string name);
    bool needsDelete();
private:
    int luaRef;
    string name;
    bool deleted=false;
    
    static Scripted* currObject;
    static lua_State* L;   
    static vector <Scripted*> list;
    static Scripted* getByInd(int ind);
    
    static int LaddToScene(lua_State *L);
    
    static int LregisterObject(lua_State *L);
    static int LloadObject(lua_State *L);
    static int LgetObject(lua_State *L);
    
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
    
    void loadFromFile(string file);
};

#endif
