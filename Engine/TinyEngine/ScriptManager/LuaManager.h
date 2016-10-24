#pragma once
#include "ThirdParty/lua_5_3_3/lua.hpp"
#include "LuaVal.h"

class LuaManager
{
public:
	static bool createInstance();
	static LuaManager* instance();
	static void destroyInstance();

	LuaVal call(const char* funcName, ...);

	bool doFile(const char* fileName);
protected:
	bool init();
	LuaManager();
	~LuaManager();

	lua_State* _LuaState;
	static LuaManager* s_instance;
};