#pragma once

#include "ThirdParty/lua_5_3_3/lua.hpp"
#include "LuaVal.h"
#include "LuaDefines.h"

class LuaManager
{
	friend class LuaFuns;
public:
	static bool createInstance();
	static LuaManager* instance();
	static void destroyInstance();

	// call a lua function. For example: 
	// LuaManager::instance()->call("add",1,2); //lua function add(a,b) can called with a == 1 and b == 2
	// std::list<LuaVal> ret = LuaManager::instance()->call("Foo",1,"abc",LuaVal{1,2,3}); // lua function Foo(a,b,c) can be called with a == 1, b == "abc", c == {1,2,3}.
	// If Foo return 123,"abc". the list ret will be ret[0] == LuaVal(123), ret[1] == LuaVal("abc")
	template<typename... Args>
	std::list<LuaVal> call(const char* funcName, Args... args);
	
	// you can push a LuaVal to the top of stack
	bool pushVal(const LuaVal& val);

	// you can push a LuaVal to the top of a given stack
	bool pushVal(lua_State* L, const LuaVal& val);

	// you can push many LuaVals to stack. For example: pushVal(1, "abc", LuaVal::NIL, LuaVal{1,2,3}).
	// Now lua stack is:
	// -1 : {1,2,3}
	// -2 : nil
	// -3 : "abc"
	// -4 : 1
	template<typename... Args>
	bool pushVal(const LuaVal& val1, Args... args);


	// get a value at index. If index is negative, -1 is stack top, -size is stack bottom. If index is non-negative, 1 is statck bottom and size is stack top
	LuaVal getVal(int index);
	LuaVal getVal(lua_State* L, int index);

	// get lua machine
	lua_State* getLuaMachine();
protected:
	// load all script file under game:Script
	bool loadFile();

	bool init();
	std::list<LuaVal> doCall(int oldStackDeep, int paramNum);
	bool pushVal();
	

	LuaManager();
	~LuaManager();

	static int printVal(lua_State* L);

	int _runningLuaFunctions;
	lua_State* _LuaState;
	static LuaManager* s_instance;
	// add all using ref so that to ensure it will not desconstruct while using
	std::vector<RefCountPtr<RefCountObj> > _usingRefObj;
};

template<typename... Args>
bool LuaManager::pushVal(const LuaVal& val1, Args... args)
{
	bool ret = pushVal(val1);
	ret = ret && pushVal(args...);
	return ret;
}

template<typename... Args>
std::list<LuaVal> LuaManager::call(const char* funcName, Args... args)
{
	++_runningLuaFunctions;
	lua_getglobal(_LuaState, funcName);
	int oldStackDeep = lua_gettop(_LuaState) - 1;
	pushVal(args...);
	std::list<LuaVal> ret = doCall(oldStackDeep, sizeof...(Args));
	--_runningLuaFunctions;
	if (_runningLuaFunctions == 0)
		_usingRefObj.clear();

	int newTop = lua_gettop(_LuaState);
	TinyAssert(newTop == oldStackDeep, "lua stack is destroyed");
	return ret;
}
