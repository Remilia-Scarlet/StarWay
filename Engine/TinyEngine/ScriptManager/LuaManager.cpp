#include "TinyEngine\precomp.h"
#include "LuaManager.h"

LuaManager* LuaManager::s_instance = nullptr;

bool LuaManager::createInstance()
{
	TinyAssert(s_instance == nullptr);
	s_instance = new LuaManager();
	if (s_instance && s_instance->init())
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

bool LuaManager::init()
{
	do 
	{
		_LuaState = luaL_newstate();
		TINY_BREAK_IF(!_LuaState);
		return true;
	} while (0);
	return false;
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
