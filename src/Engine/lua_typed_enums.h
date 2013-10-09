// Copyright (c) 2010 Tom Distler
//
// LICENSE: Do whatever you want. No warranty is implied or given. The code is
// released as-is... you're on your own.
//
#ifndef _LUA_TYPED_ENUMS_H_
#define _LUA_TYPED_ENUMS_H_

#include <lua5.2/lua.hpp>

// Adds an enumerated type into Lua.
//
// L - Lua state.
// tname - The name of the enum type.
// <name:string><value:int> pairs, terminated by a null (0).
//
// EX: Assume the following enum in C-code:
//
//  typedef enum _TYPE { TYPE_FOO=0, TYPE_BAR, TYPE_BAZ, TYPE_MAX } TYPE;
//
// To map this to Lua, do the following:
//
//  add_enum_to_lua( L, "type",
//    "foo", TYPE_FOO,
//    "bar", TYPE_BAR,
//    "baz", TYPE_BAZ,
//    0);
//
// In Lua, you can access the enum as:
//  type.foo
//  type.bar
//  type.baz
//
// You can print the actual value in Lua by:
//  > print(type.foo.value)
//
bool add_enum_to_lua(lua_State* L, const char* tname, ...);

// Validates that the specified value is the correct enumerated type.
//
// L - Lua state
// tname - The name of the expected type
// index - The stack index of the value to test
//
// EX: Assume you have a function in lua 'dosomething' which is called:
//
//  > dosomething(type.foo)
//
// Your C function gets called, and you use check_enum_type to verify the
// argument is the correct type.
//
// int lua_dosomething(lua_State *L)
// {
//   if (!check_enum_type(L, "type", -1))
//     /* error*/
//   ...
// }
//
bool check_enum_type(lua_State* L, const char* tname, int index);

// Gets the enum value as an integer.
//
// L - Lua state
// index - The index on the Lua stack where the enum resides.
//
// EX:
//
// int lua_dosomething(lua_State *L)
// {
//   if (!check_enum_type(L, "type", -1))
//     /* error*/
//   
//   int value = get_enum_value(L, -1);
//   ...
// }
//
int get_enum_value(lua_State* L, int index);

#endif // _LUA_TYPED_ENUMS_H_
