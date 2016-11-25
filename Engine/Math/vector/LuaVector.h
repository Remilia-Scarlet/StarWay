#pragma once
#include "TinyEngine/ScriptManager/LuaManager.h"

template <class ValueType, int Size>
class VectorStorage;

class LuaVector
{
	template <class ValueType, int Size>
	friend class VectorStorage;
public:
	static bool createLuaPrototype();
public:
	static int L_create(lua_State* L);
	static int L_lenth(lua_State* L);

	static int L___index(lua_State* L);
	static int L___newindex(lua_State* L);
	static int L___add(lua_State* L);
	static int L___sub(lua_State* L);
	static int L___mul(lua_State* L);
	static int L___div(lua_State* L);
protected:
	static void* allocAndPushUserData(int size);
	static void* allocAndPushUserData(int size,int metaIndex);
};