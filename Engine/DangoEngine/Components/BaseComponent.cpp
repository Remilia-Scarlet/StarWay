#include "DangoEngine/precomp.h"
#include "BaseComponent.h"
#include "DangoEngine/Objects/Object.h"
#include "DangoEngine/Engine/Engine.h"

BaseComponent::BaseComponent(const char* name)
	:_name(name)
{
}


BaseComponent::~BaseComponent()
{
}

void BaseComponent::setOwner(const Ash::RefCountPtr<Object> & owner)
{
	_owner = owner;
}

Ash::WeakRefPtr<Object> BaseComponent::getOwner() const
{
	return _owner;
}
