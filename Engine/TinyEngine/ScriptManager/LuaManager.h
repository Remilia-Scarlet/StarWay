#pragma once

#include "ThirdParty/lua_5_3_3/lua.hpp"
#include "LuaVal.h"

#define LUA_PARAM_ERROR(NAME) luaL_error(L, "function %s param error", #NAME)
static const char* CPP_LUA_POTABLE = "_ObjList";
static const char* LUA_CPP_REF_NAME = "c_ref";

// Used in prototype creating. First PROTOTYPE_PREPARE() then PROTOTYPE_REGISTER_FUN(FUN_NAME) if needed. Then PROTOTYPE_END(CLASS_NAME).
#define LUA_PROTOTYPE_PREPARE()       \
	lua_State* L = LuaManager::instance()->getLuaMachine();\
	int oldStack = lua_gettop(L); \
	lua_newtable(L);              \
	lua_pushstring(L, "__index"); \
	lua_pushnil(L);               \
	lua_copy(L, -3, -1);          \
	lua_rawset(L, -3);

#define LUA_PROTOTYPE_REGIST_FUN(FUN_NAME) do {lua_pushstring(L, #FUN_NAME);lua_pushcfunction(L, L_##FUN_NAME);lua_rawset(L, -3);} while (0)

#define LUA_PROTOTYPE_END(CLASS_NAME)\
	lua_setglobal(L, #CLASS_NAME);\
	TinyAssert(lua_gettop(L) == oldStack);

// create a lua object with prototype #_CLASS_NAME_ onto the top of stack
#define LUA_GENERATE_OBJ(CLASS_NAME)\
	lua_State* L = LuaManager::instance()->getLuaMachine();\
	int oldSize = lua_gettop(L);\
	lua_getglobal(L, CPP_LUA_POTABLE);\
	lua_pushinteger(L, lua_Integer(this));\
	lua_newtable(L);\
	lua_getglobal(L, CLASS_NAME);\
	lua_setmetatable(L, -2);\
	lua_pushstring(L, LUA_CPP_REF_NAME);\
	lua_pushlightuserdata(L, this);\
	lua_rawset(L, -3);\
	lua_rawset(L, -3);\
	lua_pop(L, 1);\
	TinyAssert(oldSize == lua_gettop(L));

#define LUA_REMOVE_OBJ()\
	do \
	{\
		if (Engine::instance()->isExiting())\
			break; \
		lua_State* L = LuaManager::instance()->getLuaMachine(); \
		int oldSize = lua_gettop(L); \
		lua_getglobal(L, CPP_LUA_POTABLE); \
		lua_pushnil(L); \
		lua_rawseti(L, -2, lua_Integer(this)); \
		lua_pop(L, 1); \
		TinyAssert(oldSize == lua_gettop(L));\
	} while (0)
	

#define LUA_CREATE_FUN_P0(CLASS_NAME)\
	static int L_create(lua_State* L)\
	{\
		LuaManager::instance()->pushVal(CLASS_NAME##::create());\
		return 1;\
	}

#define LUA_CREATE_FUN_P1(CLASS_NAME,P1_TYPE)\
	static int L_create(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		LuaManager::instance()->pushVal(CLASS_NAME##::create(p1));\
		return 1;\
	}

#define LUA_CREATE_FUN_P2(CLASS_NAME,P1_TYPE,P2_TYPE)\
	static int L_create(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P1_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		LuaManager::instance()->pushVal(CLASS_NAME##::create(p1,p2));\
		return 1;\
	}

#define LUA_CREATE_FUN_P3(CLASS_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	static int L_create(lua_State* L)\
	{\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P1_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		P1_TYPE p3 = LuaManager::instance()->getVal(L,4).convert<P3_TYPE>();\
		LuaManager::instance()->pushVal(CLASS_NAME##::create(p1,p2,p3));\
		return 1;\
	}

#define LUA_MEMBER_FUN_P0R0_DECL(CLASS_NAME,FUN_NAME)\
	static int L_##FUN_NAME(lua_State* L)

#define LUA_MEMBER_FUN_P0R0_IMPL(CLASS_NAME,FUN_NAME)\
	int CLASS_NAME##::L_##FUN_NAME(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		if(self)\
			self->##FUN_NAME();\
		return 0;\
	}

#define LUA_MEMBER_FUN_P1R0_DECL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	static int L_##FUN_NAME(lua_State* L);

#define LUA_MEMBER_FUN_P1R0_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		if(self)\
			self->##FUN_NAME(p1);\
		return 0;\
	}

#define LUA_MEMBER_FUN_P2R0_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	static int L_##FUN_NAME(lua_State* L);

#define LUA_MEMBER_FUN_P2R0_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P1_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		if(self)\
			self->##FUN_NAME(p1,p2);\
		return 0;\
	}

#define LUA_MEMBER_FUN_P3R0_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	static int L_##FUN_NAME(lua_State* L)

#define LUA_MEMBER_FUN_P3R0_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P1_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		P1_TYPE p3 = LuaManager::instance()->getVal(L,4).convert<P3_TYPE>();\
		if(self)\
			self->##FUN_NAME(p1,p2,p3);\
		return 0;\
	}

#define LUA_MEMBER_FUN_P0R1_DECL(CLASS_NAME,FUN_NAME)\
	static int L_##FUN_NAME(lua_State* L)

#define LUA_MEMBER_FUN_P0R1_IMPL(CLASS_NAME,FUN_NAME)\
	int CLASS_NAME##::L_##FUN_NAME(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		if(self)\
			LuaManager::instance()->pushVal(self->##FUN_NAME());\
		return 1;\
	}

#define LUA_MEMBER_FUN_P1R1_DECL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	static int L_##FUN_NAME(lua_State* L)

#define LUA_MEMBER_FUN_P1R1_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		if(self)\
			LuaManager::instance()->pushVal(self->##FUN_NAME(p1));\
		return 1;\
	}

#define LUA_MEMBER_FUN_P2R1_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	static int L_##FUN_NAME(lua_State* L)\

#define LUA_MEMBER_FUN_P2R1_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P1_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		if(self)\
			LuaManager::instance()->pushVal(self->##FUN_NAME(p1,p2));\
		return 1;\
	}

#define LUA_MEMBER_FUN_P3R1_DECL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	static int L_##FUN_NAME(lua_State* L)\

#define LUA_MEMBER_FUN_P3R1_IMPL(CLASS_NAME,FUN_NAME,P1_TYPE,P2_TYPE,P3_TYPE)\
	int CLASS_NAME##::L_##FUN_NAME(lua_State* L)\
	{\
		CLASS_NAME* self = LuaManager::instance()->getVal(L,1).convert<CLASS_NAME*>();\
		P1_TYPE p1 = LuaManager::instance()->getVal(L,2).convert<P1_TYPE>();\
		P1_TYPE p2 = LuaManager::instance()->getVal(L,3).convert<P2_TYPE>();\
		P1_TYPE p3 = LuaManager::instance()->getVal(L,4).convert<P3_TYPE>();\
		if(self)\
			LuaManager::instance()->pushVal(self->##FUN_NAME(p1,p2,p3));\
		return 1;\
	}

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
	pushVal(val1);
	pushVal(args...);
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
