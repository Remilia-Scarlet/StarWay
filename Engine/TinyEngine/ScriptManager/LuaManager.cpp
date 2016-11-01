#include "TinyEngine\precomp.h"
#include "LuaManager.h"
#include "Ash/FileSystem/Path_Win.h"

LuaManager* LuaManager::s_instance = nullptr;

bool LuaManager::createInstance()
{
	TinyAssert(s_instance == nullptr);
	s_instance = new LuaManager();
	if (s_instance && s_instance->init() && s_instance->loadFile())
		return true;
	TINY_SAFE_DELETE(s_instance);
	return false;
}

LuaManager* LuaManager::instance()
{
	return s_instance;
}

void LuaManager::destroyInstance()
{
	TINY_SAFE_DELETE(s_instance);
}

bool LuaManager::pushVal(const LuaVal& val)
{
	int oldStack = lua_gettop(_LuaState);
	switch (val.getType())
	{
	case LuaVal::DataType::NIL:
		lua_pushnil(_LuaState);
		break;
	case LuaVal::DataType::BOOLEAN:
		lua_pushboolean(_LuaState, val.convertBoolean());
		break;
	case LuaVal::DataType::INT64:
		lua_pushinteger(_LuaState, val.convertInt64());
		break;
	case LuaVal::DataType::DOUBLE:
		lua_pushnumber(_LuaState, val.convertDouble());
		break;
	case LuaVal::DataType::STRING:
		lua_pushstring(_LuaState, val.convertCharPointer());
		break;
	case LuaVal::DataType::REF_OBJ:
		lua_pushlightuserdata(_LuaState, val.convertRefObj());
		_usingRefObj.push_back(val.convertRefPtr<RefCountObj>());
		break;
	case LuaVal::DataType::TABLE:
		lua_newtable(_LuaState);
		for (auto& pair : val)
		{
			pushVal(pair.first);
			pushVal(pair.second);
			lua_rawset(_LuaState, -3);
		}
		break;
	default:
		return false;
	}
	int newTop = lua_gettop(_LuaState);
	if (newTop != oldStack + 1)
	{
		TinyAssert(false, "lua stack disdtroyed");
		lua_settop(_LuaState, oldStack);
		return false;
	}
	return true;
}

LuaVal LuaManager::getVal(int index)
{
	LuaVal v;
	switch (lua_type(_LuaState, index))
	{
	case LUA_TNONE:
	case LUA_TNIL:
		;//nothing to do
		break;
	case LUA_TBOOLEAN:
		v = lua_toboolean(_LuaState, 1) == 0 ? false : true;
		break;
	case LUA_TNUMBER:
		if (lua_isinteger(_LuaState, index))
			v = lua_tointeger(_LuaState, index);
		else
			v = lua_tonumber(_LuaState, index);
		break;
	case LUA_TSTRING:
		v = lua_tostring(_LuaState, index);
		break;
	case LUA_TLIGHTUSERDATA:
	case LUA_TUSERDATA:
	{
		RefCountObj* obj = (RefCountObj*)lua_touserdata(_LuaState, index);
		v = obj;
	}
	break;
	case LUA_TTABLE:
	{
		int tabPos = (index >= 0 ? index : index - 1);
		lua_pushnil(_LuaState); //key
		while (lua_next(_LuaState, tabPos) != 0)
		{
			LuaVal value = getVal(-1);
			LuaVal key = getVal(-2);
			v.setField(key, value);
			lua_pop(_LuaState, 1);
		}
	}
	break;
	case LUA_TFUNCTION:
		break;
	default:
		TinyAssert(false, "LuaManager::getVal failed");
	}
	return v;
}

bool LuaManager::loadFile()
{
	Path scriptPath("game:Script");
	std::list<Path> scripts = scriptPath.getFileList();
	for (auto& file : scripts)
	{
		if (luaL_dofile(_LuaState, file.getAbsolutePath().c_str()))
		{
			DebugString("Lua error : %s", lua_tostring(_LuaState, -1));
			lua_pop(_LuaState, -1);
			return false;
		}
	}
	return true;
}

bool LuaManager::init()
{
	do 
	{
		_LuaState = luaL_newstate();
		TINY_BREAK_IF(!_LuaState);
		luaL_openlibs(_LuaState);

		lua_register(_LuaState, "Print", printVal);
		return true;
	} while (0);
	return false;
}

std::list<LuaVal> LuaManager::doCall(int oldStackDeep, int paramNum)
{
	std::list<LuaVal> ret;
	int luaret = lua_pcall(_LuaState, paramNum, LUA_MULTRET, 0);
	if (luaret)
	{
		DebugString("Lua error:%s", lua_tostring(_LuaState, -1));
		lua_pop(_LuaState, 1);
	}
	int resultSize = lua_gettop(_LuaState) - oldStackDeep;
	if (resultSize < 0)
		TinyAssert(false, "lua stack is destroyed");

	for (int i = 0; i < resultSize; i++)
	{
		LuaVal v = getVal(-1);
		ret.insert(ret.begin(), v);
		lua_pop(_LuaState, 1);
	}
	return ret;
}

LuaManager::LuaManager()
	:_LuaState (nullptr)
{

}

LuaManager::~LuaManager()
{
	if (_LuaState)
		lua_close(_LuaState);
	_LuaState = nullptr;
}

int LuaManager::printVal(lua_State* L)
{
	int num = lua_gettop(L);
	if (num == 0)
		return 0;

	for (int i = 1; i <= num; ++i)
	{
		LuaVal val = LuaManager::instance()->getVal(i);
		DebugString("Lua info:%s", val.toString().c_str());
	}
	return 0;
}
