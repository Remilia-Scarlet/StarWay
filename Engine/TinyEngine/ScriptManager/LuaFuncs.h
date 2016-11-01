#pragma once
#include "ThirdParty/lua_5_3_3/lua.hpp"
class LuaFuns
{
public:
	static LuaFuns* instance();
	static int CreateScene(lua_State* L);
	static int CreateObject(lua_State* L);

	bool registerFuncsToLua();
private:
	std::map<const char*, lua_CFunction> _funcs;
};

