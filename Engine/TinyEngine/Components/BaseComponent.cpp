#include "precomp.h"
#include "BaseComponent.h"
#include "TinyEngine\Objects\Object.h"
#include "TinyEngine\Engine\Engine.h"

BaseComponent::BaseComponent(const char* luaPrototypeName)
	:_name(luaPrototypeName)
{
	LUA_GENERATE_OBJ(luaPrototypeName);
}


BaseComponent::~BaseComponent()
{
	LUA_REMOVE_OBJ();
}

void BaseComponent::setOwner(const RefCountPtr<Object> & owner)
{
	_owner = owner;
	
	lua_State* L = LuaManager::instance()->getLuaMachine();
	LuaManager::instance()->pushVal(L, this);
	LuaManager::instance()->pushVal(L, COMPONENT_PARENT);
	LuaManager::instance()->pushVal(owner);
	lua_rawset(L, -3);
	lua_pop(L, 1);
}

WeakRefPtr<Object> BaseComponent::getOwner() const
{
	return _owner;
}
