#include "TinyEngine\precomp.h"
#include "BaseComponent.h"
#include "TinyEngine\Objects\Object.h"
#include "TinyEngine\Engine\Engine.h"

BaseComponent::BaseComponent(const char* luaPrototypeName)
	:_id(genericObjectId())
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
}

WeakRefPtr<Object> BaseComponent::getOwner()
{
	return _owner;
}
