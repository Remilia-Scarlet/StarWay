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
	
	// you can push a value to the top of stack
	template<typename T>
	bool pushVal(const T& val);

	// you can push a LuaVal to the top of stack
	inline bool pushVal(const LuaVal& val);

	// you can push a value to the top of a given stack
	template<typename T>
	bool pushVal(lua_State* L, const T& val);

	// you can push a LuaVal to the top of a given stack
	inline bool pushVal(lua_State* L, const LuaVal& val);

	// you can push many LuaVals to stack. For example: pushVal(1, "abc", LuaVal::NIL, LuaVal{1,2,3}).
	// Now lua stack is:
	// -1 : {1,2,3}
	// -2 : nil
	// -3 : "abc"
	// -4 : 1
	template<typename... Args>
	bool pushVal(const LuaVal& val1, const Args& ... args);

	template<typename T, typename... Args>
	bool pushVal(const T& val1, const Args& ... args);


	// get a value at index. If index is negative, -1 is stack top, -size is stack bottom. If index is non-negative, 1 is statck bottom and size is stack top
	LuaVal getVal(int index);
	LuaVal getVal(lua_State* L, int index);

	// try to conver a value at index to a certain type.
	template<class T>
	typename std::remove_reference<T>::type getVal(int index);
	template<class T>
	typename std::remove_reference<T>::type getVal(lua_State* L, int index);

	// get lua machine
	lua_State* getLuaMachine();

	// format a value to string at index of stack
	std::string toString(int index);
protected:
	// load all script file under game:Script
	bool loadFile();

	bool init();
	std::list<LuaVal> doCall(int oldStackDeep, int paramNum);

	inline bool pushVal();
	
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

protected:
	template <class T>
	struct CharArr2CharP
	{
		typedef T type;
	};

	template<size_t ArrSize>
	struct CharArr2CharP<char[ArrSize]>
	{
		typedef char* type;
	};

	template<>
	struct CharArr2CharP<char[]>
	{
		typedef char* type;
	};


	enum class LMConverterType
	{
		OTHER = -1,
		NIL = 1,
		BOOLEAN,
		FLOAT_POINTER_NUMBER,
		INTEGRAL_NUMBER,
		STRING,
		REF_OBJ,
		LUAVAL
	};

	template<LMConverterType Type, class TAR>
	struct LMConverter
	{
		static typename std::remove_reference<TAR>::type getVal(lua_State* L, int index);
		static bool pushVal(lua_State* L, const TAR& val);
	};

	template<class TAR>
	struct LMConverter<LMConverterType::NIL, TAR> //null
	{
		static bool pushVal(lua_State* L, std::nullptr_t);
	};

	template<class TAR>
	struct LMConverter<LMConverterType::BOOLEAN, TAR> //bool
	{
		static bool getVal(lua_State* L, int index);
		static bool pushVal(lua_State* L, bool val);
	};

	template<class TAR>
	struct LMConverter<LMConverterType::FLOAT_POINTER_NUMBER, TAR> //float point number
	{
		static typename std::remove_reference<TAR>::type getVal(lua_State* L, int index);
		static bool pushVal(lua_State* L, lua_Number val);
	};

	template<class TAR>
	struct LMConverter<LMConverterType::INTEGRAL_NUMBER, TAR> //integral number
	{
		static typename std::remove_reference<TAR>::type getVal(lua_State* L, int index);
		static bool pushVal(lua_State* L, lua_Integer val);
	};

	template<class TAR>
	struct LMConverter<LMConverterType::STRING, TAR> //string
	{
		static typename std::remove_reference<typename CharArr2CharP<TAR>::type>::type getVal(lua_State* L, int index);
		static bool pushVal(lua_State* L, const char* val);
		static bool pushVal(lua_State* L, const std::string& val);
	};

	template<class TAR>
	struct LMConverter<LMConverterType::REF_OBJ, TAR> //ref obj
	{
		static typename std::remove_reference<TAR>::type getVal(lua_State* L, int index);
		static bool pushVal(lua_State* L, RefCountObj* val);
		template<typename RefType>
		static bool pushVal(lua_State* L, const RefCountPtr<RefType>& val);
	};

	template<class TAR>
	struct LMConverter<LMConverterType::LUAVAL, TAR> //luaval
	{
		static LuaVal getVal(lua_State* L, int index);
		static bool pushVal(lua_State* L, const LuaVal& val);
	};
};

#include "LuaManager.inl"
