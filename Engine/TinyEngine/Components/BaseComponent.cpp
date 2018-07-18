#include "TinyEngine/precomp.h"
#include "BaseComponent.h"
#include "TinyEngine\Objects\Object.h"
#include "TinyEngine\Engine\Engine.h"

BaseComponent::BaseComponent(const char* name)
	:_name(name)
{
}


BaseComponent::~BaseComponent()
{
}

void BaseComponent::setOwner(const RefCountPtr<Object> & owner)
{
	_owner = owner;
}

WeakRefPtr<Object> BaseComponent::getOwner() const
{
	return _owner;
}
