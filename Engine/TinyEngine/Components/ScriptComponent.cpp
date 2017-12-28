#include "precomp.h"
#include "ScriptComponent.h"
#include "TinyEngine\ScriptManager\LuaManager.h"
#include "TinyEngine\Objects\Object.h"

void ScriptComponent::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(create);

	LUA_PROTOTYPE_END(ScriptComponent);
}

ScriptComponentPtr ScriptComponent::create(const std::string& luaFunctionTable)
{
	ScriptComponent* ret = new ScriptComponent();
	if (ret && ret->init(luaFunctionTable))
		return ScriptComponentPtr(ret);
	TINY_SAFE_DELETE(ret);
	return ScriptComponentPtr();
}

void ScriptComponent::setOwner(const RefCountPtr<Object> & owner)
{
	BaseComponent::setOwner(owner);

	lua_State* L = LuaManager::instance()->getLuaMachine();
	lua_getglobal(L, _luaFunctionTable.c_str());
	lua_pushstring(L, SCRIPT_COMPONENT_TABLE_OBJ);
	LuaManager::instance()->pushVal(L, owner);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

void ScriptComponent::update(float dt)
{
	BaseComponent::update(dt);

	lua_State* L = LuaManager::instance()->getLuaMachine();
	int oldTop = lua_gettop(L);
	int type = lua_getglobal(L, _luaFunctionTable.c_str());
	TinyAssert(type == LUA_TTABLE);

	//call init
	if (!_inited)
	{
		_inited = true;

		lua_pushstring(L, LUA_SCRIPT_INIT_FUN_NAME);
		type = lua_gettable(L, -2);
		if (type == LUA_TFUNCTION)
		{
			lua_pushnil(L);
			lua_copy(L, -3, -1);
			LuaManager::instance()->call(-2);
		}
		else
			lua_pop(L, 1);
	}
	TinyAssert(oldTop + 1 == lua_gettop(L));

	//call update
	{
		lua_pushstring(L, LUA_SCRIPT_UPDATE_FUN_NAME);
		type = lua_gettable(L, -2);
		if (type == LUA_TFUNCTION)
		{
			lua_pushnil(L);
			lua_copy(L, -3, -1);
			lua_pushnumber(L, dt);
			LuaManager::instance()->call(-3);
		}
		else
			lua_pop(L, 1);
	}

	lua_pop(L, 1);
	TinyAssert(oldTop == lua_gettop(L));
}

bool ScriptComponent::init(const std::string& luaFunctionTable)
{
	do 
	{
		_luaFunctionTable = luaFunctionTable;
		int type = lua_getglobal(LuaManager::instance()->getLuaMachine(), _luaFunctionTable.c_str());
		lua_pop(LuaManager::instance()->getLuaMachine(), 1);
		TINY_BREAK_IF(type == LUA_TNIL);
		return true;
	} while (0);
	return false;
}

ScriptComponent::ScriptComponent()
	:BaseComponent(TO_STRING(ScriptComponent))
	, _inited(false)
{

}

ScriptComponent::~ScriptComponent()
{

}
