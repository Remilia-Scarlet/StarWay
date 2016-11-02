#include "TinyEngine\precomp.h"
#include "ScriptComponent.h"
#include "TinyEngine\ScriptManager\LuaManager.h"

ScriptComponentPtr ScriptComponent::create(const std::string& luaFunctionTable)
{
	ScriptComponent* ret = new ScriptComponent();
	if (ret && ret->init(luaFunctionTable))
		return ScriptComponentPtr(ret);
	TINY_SAFE_DELETE(ret);
	return ScriptComponentPtr();
}

void ScriptComponent::update(float dt)
{
	BaseComponent::update(dt);
	LuaVal callFunc = {"ObjUpdate","update"};
	LuaManager::instance()->call("CppCallLua", callFunc);
}

bool ScriptComponent::init(const std::string& luaFunctionTable)
{
	do 
	{
		_luaFunctionTable = luaFunctionTable;
		return true;
	} while (0);
	return false;
}

ScriptComponent::ScriptComponent()
{

}

ScriptComponent::~ScriptComponent()
{

}
