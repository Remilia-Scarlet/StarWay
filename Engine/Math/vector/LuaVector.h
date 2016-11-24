#pragma once
#include "Vector.h"
#include "TinyEngine/ScriptManager/LuaManager.h"

class LuaVector
{
public:
	static bool createLuaPrototype();
public:
	static int L_create(lua_State* L);
	static int L_x(lua_State* L);
	static int L_y(lua_State* L);
	static int L_z(lua_State* L);
	static int L_w(lua_State* L);


	static int L_add(lua_State* L);
protected:
	static void* allocAndPushUserData(int size);
};