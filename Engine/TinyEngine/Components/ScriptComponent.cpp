#include "TinyEngine\precomp.h"
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
	LuaVal callFunc = { _luaFunctionTable.c_str(),"update"};
	LuaManager::instance()->call(CPP_CALL_LUA_NAME, callFunc, dt);
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
{

}

ScriptComponent::~ScriptComponent()
{

}
