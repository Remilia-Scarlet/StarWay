#include "LuaManager.h"

template<LuaManager::LMConverterType Type, class TAR>
bool LuaManager::LMConverter<Type, TAR>::pushVal(lua_State* L, const TAR& val)
{
	return TAR::pushToLua(L, val);
}

template<typename TAR> 
bool LuaManager::LMConverter<LuaManager::LMConverterType::NIL, TAR>::pushVal(lua_State* L, std::nullptr_t)
{
	lua_pushnil(L);
	return true;
}

template<typename TAR>
bool LuaManager::LMConverter<LuaManager::LMConverterType::BOOLEAN, TAR>::pushVal(lua_State* L, bool val)
{
	lua_pushboolean(L, val ? 1 : 0);
	return true;
}

template<typename TAR>
bool LuaManager::LMConverter<LuaManager::LMConverterType::FLOAT_POINTER_NUMBER, TAR>::pushVal(lua_State* L, lua_Number val)
{
	lua_pushnumber(L, val);
	return true;
}

template<typename TAR>
bool LuaManager::LMConverter<LuaManager::LMConverterType::INTEGRAL_NUMBER, TAR>::pushVal(lua_State* L, lua_Integer val)
{
	lua_pushinteger(L, val);
	return true;
}

template<typename TAR>
bool LuaManager::LMConverter<LuaManager::LMConverterType::STRING, TAR>::pushVal(lua_State* L, const char* val)
{
	lua_pushstring(L, val);
	return true;
}

template<typename TAR>
bool LuaManager::LMConverter<LuaManager::LMConverterType::STRING, TAR>::pushVal(lua_State* L, const std::string& val)
{
	return pushVal(L, val.c_str());
}

template<typename TAR>
bool LuaManager::LMConverter<LuaManager::LMConverterType::REF_OBJ, TAR>::pushVal(lua_State* L, RefCountObj* val)
{
	lua_getglobal(L, CPP_LUA_POTABLE);
	lua_rawgeti(L, -1, lua_Integer(val));
	lua_replace(L, -2);
	LuaManager::instance()->_usingRefObj.push_back(RefCountPtr<RefCountObj>(val));
	return true;
}

template<typename TAR>
template<typename RefType>
bool LuaManager::LMConverter<LuaManager::LMConverterType::REF_OBJ, TAR>::pushVal(lua_State* L, const RefCountPtr<RefType>& val)
{
	return pushVal(L, val.get());
}

template<typename TAR>
bool LuaManager::LMConverter<LuaManager::LMConverterType::LUAVAL, TAR>::pushVal(lua_State* L, const LuaVal& val)
{
	int oldStack = lua_gettop(L);
	switch (val.getType())
	{
	case LuaVal::DataType::NIL:
		LMConverter<LMConverterType::NIL, std::nullptr_t>::pushVal(L, nullptr);
		break;
	case LuaVal::DataType::BOOLEAN:
		LMConverter<LMConverterType::BOOLEAN, bool>::pushVal(L, val.convertBoolean());
		break;
	case LuaVal::DataType::INT64:
		LMConverter<LMConverterType::INTEGRAL_NUMBER, int64_t>::pushVal(L, val.convertInt64());
		break;
	case LuaVal::DataType::DOUBLE:
		LMConverter<LMConverterType::FLOAT_POINTER_NUMBER, double>::pushVal(L, val.convertDouble());
		break;
	case LuaVal::DataType::STRING:
		LMConverter<LMConverterType::STRING, const char*>::pushVal(L, val.convertCharPointer());
		break;
	case LuaVal::DataType::REF_OBJ:
		LMConverter<LMConverterType::REF_OBJ, RefCountObj*>::pushVal(L, val.convertRefObj());
		break;
	case LuaVal::DataType::TABLE:
		if (val.isLuaRefTable())
		{
			int type = lua_getglobal(L, LUAVAL_TABLE);
			TinyAssert(type == LUA_TTABLE);
			lua_geti(L, -1, val.getLuaRefTableId());
			lua_remove(L, -2);
		}
		else
		{
			lua_newtable(L);
			for (auto it = val.begin(); it != val.end(); ++it)
			{
				LuaManager::instance()->pushVal(it.key());
				LuaManager::instance()->pushVal(it.val());
				lua_rawset(L, -3);
			}
		}
		break;
	default:
		TinyAssert(false, "unreachable code");
		return false;
	}
	int newTop = lua_gettop(L);
	if (newTop != oldStack + 1)
	{
		TinyAssert(false, "lua stack disdtroyed");
		lua_settop(L, oldStack);
		return false;
	}
	return true;
}



template<typename T>
bool LuaManager::pushVal(const T& val)
{
	return pushVal(_LuaState, val);
}


inline bool LuaManager::pushVal(const LuaVal& val)
{
	return pushVal(_LuaState, val);
}

template<typename T>
inline bool LuaManager::pushVal(lua_State * L, const T & val)
{
	constexpr bool isNil = std::is_same<T, std::nullptr_t>::value;
	constexpr bool isBool = std::is_same<T, bool>::value;
	constexpr bool isFloatPoint = std::is_floating_point<T>::value;
	constexpr bool isIntegral = std::is_integral<T>::value;
	constexpr bool isString = std::is_same<T, const char*>::value || (std::is_same<typename std::remove_all_extents<T>::type, char>::value && std::is_array<T>::value) || std::is_same<T, std::string>::value;
	constexpr bool isPointer = std::is_pointer<typename GetRefPtrInner<std::remove_cv<std::remove_reference<typename T>::type>::type>::type>::value;
	constexpr bool isLuaVal = std::is_same<typename std::remove_cv<typename std::remove_reference<T>::type>::type, LuaVal>::value;

	constexpr LuaManager::LMConverterType type =
		(isNil ? LuaManager::LMConverterType::NIL
			: (isBool ? LuaManager::LMConverterType::BOOLEAN
				: (isFloatPoint ? LuaManager::LMConverterType::FLOAT_POINTER_NUMBER
					: (isIntegral ? LuaManager::LMConverterType::INTEGRAL_NUMBER
						: (isString ? LuaManager::LMConverterType::STRING
							: (isPointer ? LuaManager::LMConverterType::REF_OBJ
								: ( isLuaVal ? LuaManager::LMConverterType::LUAVAL
									: LuaManager::LMConverterType::OTHER)
								)
							)
						)
					)
				)
			);

	return LuaManager::LMConverter<type, T>::pushVal(L, val);
}


inline bool LuaManager::pushVal(lua_State* L, const LuaVal& val)
{
	return pushVal<LuaVal>(L, val);
}

template<typename... Args>
bool LuaManager::pushVal(const LuaVal& val1, const Args&... args)
{
	bool ret = pushVal(val1);
	ret = ret && pushVal(args...);
	return ret;
}

template<typename T, typename ...Args>
bool LuaManager::pushVal(const T& val1, const Args& ...args)
{
	bool ret = pushVal(val1);
	ret = ret && pushVal(args...);
	return ret;
}


inline bool LuaManager::pushVal()
{
	return true;
}

template<LuaManager::LMConverterType Type, class TAR>
typename std::remove_reference<TAR>::type LuaManager::LMConverter<Type,TAR>::getVal(lua_State* L,int index)
{
	return std::remove_reference<TAR>::type::createFromLua(L, index);
}


template<class TAR>
bool LuaManager::LMConverter<LuaManager::LMConverterType::BOOLEAN, TAR>::getVal(lua_State* L, int index)
{
	if (lua_type(L, index) != LUA_TBOOLEAN)
	{
		TinyAssert(false, "can't convert");
		return false;
	}
	return lua_toboolean(L, index) == 1 ? true : false;
}


template<class TAR>
typename std::remove_reference<TAR>::type LuaManager::LMConverter<LuaManager::LMConverterType::FLOAT_POINTER_NUMBER, TAR>::getVal(lua_State* L, int index)
{
	if (lua_type(L, index) != LUA_TNUMBER)
	{
		TinyAssert(false, "can't convert");
		return 0;
	}
	return (typename std::remove_reference<TAR>::type)lua_tonumber(L, index);
}

template<class TAR>
typename std::remove_reference<TAR>::type LuaManager::LMConverter<LuaManager::LMConverterType::INTEGRAL_NUMBER, TAR>::getVal(lua_State* L, int index)
{
	if (lua_type(L, index) != LUA_TNUMBER)
	{
		TinyAssert(false, "can't convert");
		return 0;
	}
	return (typename std::remove_reference<TAR>::type)lua_tointeger(L, index);
}


template<class TAR>
typename std::remove_reference<typename LuaManager::CharArr2CharP<TAR>::type>::type LuaManager::LMConverter<LuaManager::LMConverterType::STRING, TAR>::getVal(lua_State* L, int index)
{
	if (lua_type(L, index) != LUA_TSTRING)
	{
		TinyAssert(false, "can't convert");
		return "";
	}
	return typename std::remove_reference<TAR>::type(lua_tostring(L, index));
}

template<class TAR>
typename std::remove_reference<TAR>::type LuaManager::LMConverter<LuaManager::LMConverterType::REF_OBJ, TAR>::getVal(lua_State* L, int index)
{
	typedef typename GetRefPtrInner<typename std::remove_reference<TAR>::type>::type PurePtrType;
	PurePtrType ptr = nullptr;
	if (lua_type(L, index) == LUA_TLIGHTUSERDATA)
	{
		ptr = dynamic_cast<PurePtrType>((RefCountObj*)lua_touserdata(L, index));
	}
	else if (lua_type(L, index) == LUA_TTABLE)
	{
		int oldTop = lua_gettop(L);
		if (lua_getfield(L, index, LUA_CPP_REF_NAME) == LUA_TLIGHTUSERDATA)
		{
			ptr = dynamic_cast<PurePtrType>((RefCountObj*)lua_touserdata(L, -1));
		}
		lua_pop(L, 1);
		TinyAssert(oldTop == lua_gettop(L));
	}
	if (ptr != nullptr)
		return typename std::remove_reference<TAR>::type(ptr);
	else
	{
		TinyAssert(false, "can't convert");
		return nullptr;
	}
}

template<class TAR>
LuaVal LuaManager::LMConverter<LuaManager::LMConverterType::LUAVAL, TAR>::getVal(lua_State* L, int index)
{
	LuaVal v;
	switch (lua_type(L, index))
	{
	case LUA_TNONE:
	case LUA_TNIL:
		;//nothing to do
		break;
	case LUA_TBOOLEAN:
		v = LuaManager::LMConverter<LuaManager::LMConverterType::BOOLEAN, bool>::getVal(L, index);
		break;
	case LUA_TNUMBER:
		if (lua_isinteger(L, index))
			v = LuaManager::LMConverter<LuaManager::LMConverterType::INTEGRAL_NUMBER, lua_Integer>::getVal(L, index);
		else
			v = LuaManager::LMConverter<LuaManager::LMConverterType::FLOAT_POINTER_NUMBER, lua_Number>::getVal(L, index);
		break;
	case LUA_TSTRING:
		v = LuaManager::LMConverter<LuaManager::LMConverterType::STRING, const char*>::getVal(L, index);
		break;
	case LUA_TLIGHTUSERDATA:
		v = LuaManager::LMConverter<LuaManager::LMConverterType::REF_OBJ, RefCountObj*>::getVal(L, index);
		break;
	case LUA_TUSERDATA:
		{
			int oldTop = lua_gettop(L);
			int tabIndex = lua_absindex(L, index);
			int type = lua_getglobal(L, LUAVAL_TABLE);
			TinyAssert(type == LUA_TTABLE);
			int64_t index = ++LuaManager::s_luaTableIndex;
			lua_pushinteger(L, index);
			lua_pushnil(L);
			lua_copy(L, tabIndex, -1);
			lua_settable(L, -3);
			v.setUserData(index);
			lua_pop(L, 1);
			TinyAssert(oldTop == lua_gettop(L));
		}
		break;
	case LUA_TTABLE:
		{
			int oldTop = lua_gettop(L);
			if (lua_getfield(L, index, LUA_CPP_REF_NAME) == LUA_TLIGHTUSERDATA)
			{
				RefCountObj* obj = (RefCountObj*)lua_touserdata(L, -1);
				v = obj;
				lua_pop(L, 1);
			}
			else
			{
				lua_pop(L, 1);
				int tabIndex = lua_absindex(L, index);
				int type = lua_getglobal(L, LUAVAL_TABLE);
				TinyAssert(type == LUA_TTABLE);
				int64_t index = ++s_luaTableIndex;
				lua_pushinteger(L, index);
				lua_pushnil(L);
				lua_copy(L, tabIndex, -1);
				lua_settable(L, -3);
				v.setRefTable(index);
				lua_pop(L, 1);
			}
			TinyAssert(oldTop == lua_gettop(L));
		}
		break;
	case LUA_TFUNCTION:
		break;
	default:
		TinyAssert(false, "LuaManager::instance()->getVal failed");
	}
	return v;
}


template<class T>
typename std::remove_reference<T>::type LuaManager::getVal(int index)
{
	return getVal<T>(_LuaState, index);
}

template<class T>
typename std::remove_reference<T>::type LuaManager::getVal(lua_State* L, int index)
{
	constexpr bool isBool = std::is_same<std::remove_cv<std::remove_reference<typename T>::type>::type, bool>::value;
	constexpr bool isFloatPoint = std::is_floating_point<typename T>::value;
	constexpr bool isIntegral = std::is_integral<typename T>::value;
	constexpr bool isString = std::is_same<typename T, const char*>::value || std::is_same<std::remove_cv<std::remove_reference<typename T>::type>::type, std::string>::value;
	constexpr bool isPointer = std::is_pointer<typename GetRefPtrInner<std::remove_cv<std::remove_reference<typename T>::type>::type>::type>::value;
	constexpr bool isLuaVal = std::is_same<LuaVal, typename std::remove_cv<std::remove_reference<typename T>::type>::type>::value;

	constexpr LuaManager::LMConverterType type =
		(isBool ? LuaManager::LMConverterType::BOOLEAN
			: (isFloatPoint ? LuaManager::LMConverterType::FLOAT_POINTER_NUMBER
				: (isIntegral ? LuaManager::LMConverterType::INTEGRAL_NUMBER
					: (isString ? LuaManager::LMConverterType::STRING
						: (isPointer ? LuaManager::LMConverterType::REF_OBJ
							: (isLuaVal ? LuaManager::LMConverterType::LUAVAL
								: LuaManager::LMConverterType::OTHER
								)
							)
						)
					)
				)
			);

	return LuaManager::LMConverter<type, T>::getVal(L, index);
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

