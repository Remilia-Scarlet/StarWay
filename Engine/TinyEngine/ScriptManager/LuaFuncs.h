#pragma once
#include "ThirdParty/lua_5_3_3/lua.hpp"
#include "LuaVal.h"
class LuaFuns
{
	friend class LuaManager;
public:
	static LuaFuns* instance();
public:
	static int GenerateCubeMesh(lua_State* L);
	static int GenerateSphereMesh(lua_State* L);
	static int StartScene(lua_State* L);
private:
	bool registerFuncsToLua();
	static bool pushVal(lua_State * L, const LuaVal& val);
	static LuaVal getVal(lua_State * L, int index);
};

