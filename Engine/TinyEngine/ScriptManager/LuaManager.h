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
	std::list<LuaVal> call(const char* funcName, const Args& ... args);
	
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
	bool pushVal(const LuaVal& val1, const Args& ... args);


	// get a value at index. If index is negative, -1 is stack top, -size is stack bottom. If index is non-negative, 1 is statck bottom and size is stack top
	LuaVal getVal(int index);
	LuaVal getVal(lua_State* L, int index);

	// try to conver a value at index to a certain type.
	template<class T>
	typename std::remove_reference<T>::type convert(int index);
	template<class T>
	typename std::remove_reference<T>::type convert(lua_State* L, int index);

	// get lua machine
	lua_State* getLuaMachine();

	// format a value to string at index of stack
	std::string toString(int index);
protected:
	// load all script file under game:Script
	bool loadFile();

	bool init();
	std::list<LuaVal> doCall(int oldStackDeep, int paramNum);
	bool pushVal();
	
	std::string doToString(int index, int deep);

	LuaManager();
	~LuaManager();

	static int printVal(lua_State* L);

	int _runningLuaFunctions;
	lua_State* _LuaState;
	static LuaManager* s_instance;
	// add all using ref so that to ensure it will not desconstruct while using
	std::vector<RefCountPtr<RefCountObj> > _usingRefObj;
	static int64_t s_luaTableIndex;
};

template<typename... Args>
bool LuaManager::pushVal(const LuaVal& val1,const Args&... args)
{
	bool ret = pushVal(val1);
	ret = ret && pushVal(args...);
	return ret;
}

template<int T, class TAR>
struct LMConverter
{
	static typename std::remove_reference<TAR>::type convert(lua_State* L,int index)
	{
		return typename std::remove_reference<TAR>::type::createFromLua(L, index);
	}
};

template<class TAR>
struct LMConverter<1, TAR> //bool
{
	static bool convert(lua_State* L, int index)
	{
		if (lua_type(L, index) != LUA_TBOOLEAN)
		{
			TinyAssert(false, "can't convert");
			return false;
		}
		return lua_toboolean(L, index) == 1 ? true : false;
	}
};

template<class TAR>
struct LMConverter<2, TAR> //integral number
{
	static TAR convert(lua_State* L, int index)
	{
		if (lua_type(L, index) != LUA_TNUMBER)
		{
			TinyAssert(false, "can't convert");
			return 0;
		}
		return TAR(lua_tonumber(L, index));
	}
};

template<class TAR>
struct LMConverter<3, TAR> //float point number
{
	static TAR convert(lua_State* L, int index)
	{
		if (lua_type(L, index) != LUA_TNUMBER)
		{
			TinyAssert(false, "can't convert");
			return 0;
		}
		return lua_tointeger(L, index);
	}
};

template<class TAR>
struct LMConverter<4, TAR> //string
{
	static typename std::remove_reference<TAR>::type convert(lua_State* L, int index)
	{
		if (lua_type(L, index) != LUA_TSTRING)
		{
			TinyAssert(false, "can't convert");
			return 0;
		}
		return typename std::remove_reference<TAR>::type(lua_tostring(L, index));
	}
};

template<class TAR>
struct LMConverter<5, TAR> //ref obj
{
	static typename std::remove_reference<TAR>::type convert(lua_State* L, int index)
	{
		if (lua_type(L, index) != LUA_TLIGHTUSERDATA)
		{
			TinyAssert(false, "can't convert");
			return 0;
		}
		return typename std::remove_reference<TAR>::type(lua_touserdata(L, index));
	}
};

template<class T>
typename std::remove_reference<T>::type LuaManager::convert(int index)
{
	return convert<T>(_LuaState, index);
}

template<class T>
typename std::remove_reference<T>::type LuaManager::convert(lua_State* L, int index)
{
	constexpr bool isBool = std::is_same<std::remove_cv<std::remove_reference<typename T>::type>::type, bool>::value;
	constexpr bool isFloatPoint = std::is_floating_point<typename T>::value;
	constexpr bool isIntegral = std::is_integral<typename T>::value;
	constexpr bool isString = std::is_same<typename T, const char*>::value || std::is_same<std::remove_cv<std::remove_reference<typename T>::type>::type, std::string>::value;
	constexpr bool isPointer = std::is_pointer<typename GetRefPtrInner<std::remove_cv<std::remove_reference<typename T>::type>::type>::type>::value;

	constexpr int type =
		(isBool ? 1
			: (isFloatPoint ? 2
				: (isIntegral ? 3
					: (isString ? 4
						: (isPointer ? 5
							: -1)
						)
					)
				)
			);

	return LMConverter<type, T>::convert(L, index);
}

template<typename... Args>
typename std::list<LuaVal> LuaManager::call(const char* funcName, const Args& ... args)
{
	int type = lua_getglobal(_LuaState, funcName);
	if (type == LUA_TFUNCTION)
	{
		++_runningLuaFunctions;
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
	else
	{
		lua_pop(_LuaState, 1);
		DebugString("Lua error : Can't call \"%s\" because it's nil", funcName);
		return std::list<LuaVal>();
	}
}
