/**
 * @(#) Scripted.h
 */

#ifndef SCRIPTED_H_H
#define SCRIPTED_H_H

#include <lua.h>

#include "../Math/Vector3.h"

#include "Tile.h"
#include <Script/ScriptManager.h>
#include <map>
#include <string>
class Scripted: public Tile
{
public:
  Scripted(State* state, string file, ScriptManager* scriptManager, Vector3 p=0, Vector3 s=Vector3(1,1,1), Vector3 r=0);
  Scripted(State* state, MXML::Tag &code, ScriptManager* scriptManager);
  ~Scripted();
  virtual void fromXML( MXML::Tag &code, ScriptManager* scriptManager);
  virtual MXML::Tag toXML( );
  edType getType();
  string getName();
  string getPath();
  static string getTemplate(string name);
  bool needsDelete();
  int luaRef;
  string name;
  bool deleted=false;
private:
  ScriptManager* scriptMan;
};

#endif
