#include "TinyEngine\precomp.h"
#include "LuaManager.h"
#include "Ash/FileSystem/Path_Win.h"
#include <functional>
#include "LuaFuncs.h"

LuaManager* LuaManager::s_instance = nullptr;
class LuaFuns;
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
	return pushVal(_LuaState, val);
}

bool LuaManager::pushVal(lua_State* L, const LuaVal& val)
{
	int oldStack = lua_gettop(L);
	switch (val.getType())
	{
	case LuaVal::DataType::NIL:
		lua_pushnil(L);
		break;
	case LuaVal::DataType::BOOLEAN:
		lua_pushboolean(L, val.convertBoolean());
		break;
	case LuaVal::DataType::INT64:
		lua_pushinteger(L, val.convertInt64());
		break;
	case LuaVal::DataType::DOUBLE:
		lua_pushnumber(L, val.convertDouble());
		break;
	case LuaVal::DataType::STRING:
		lua_pushstring(L, val.convertCharPointer());
		break;
	case LuaVal::DataType::REF_OBJ:
		lua_getglobal(L, CPP_LUA_POTABLE);
		lua_rawgeti(L, -1, lua_Integer(val.convertRefObj()));
		lua_replace(L, -2);
		_usingRefObj.push_back(val.convertRefPtr_static<RefCountObj>());
		break;
	case LuaVal::DataType::TABLE:
		lua_newtable(L);
		for (auto& pair : val)
		{
			pushVal(pair.first);
			pushVal(pair.second);
			lua_rawset(L, -3);
		}
		break;
	default:
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

bool LuaManager::pushVal()
{
	return true;
}
LuaVal LuaManager::getVal(int index)
{
	return getVal(_LuaState, index);
}

lua_State* LuaManager::getLuaMachine()
{
	return _LuaState;
}

LuaVal LuaManager::getVal(lua_State* L, int index)
{
	LuaVal v;
	switch (lua_type(L, index))
	{
	case LUA_TNONE:
	case LUA_TNIL:
		;//nothing to do
		break;
	case LUA_TBOOLEAN:
		v = lua_toboolean(L, index) == 0 ? false : true;
		break;
	case LUA_TNUMBER:
		if (lua_isinteger(L, index))
			v = lua_tointeger(L, index);
		else
			v = lua_tonumber(L, index);
		break;
	case LUA_TSTRING:
		v = lua_tostring(L, index);
		break;
	case LUA_TLIGHTUSERDATA:
	case LUA_TUSERDATA:
		{
			RefCountObj* obj = (RefCountObj*)lua_touserdata(L, index);
			v = obj;
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
				v = {};
				lua_pop(L, 1);
				lua_pushnil(L); //key
				int tabPos = (index >= 0 ? index : index - 1);
				while (lua_next(L, tabPos) != 0)
				{
					LuaVal key = getVal(L, -2);
					if (!(key.isString() && key == "__index"))
					{
						LuaVal value = getVal(L, -1);
						v.setField(key, value);
					}
					lua_pop(L, 1);
				}
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
		lua_newtable(_LuaState);
		lua_setglobal(_LuaState, CPP_LUA_POTABLE);

		LuaFuns::instance()->registerFuncsToLua();
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
