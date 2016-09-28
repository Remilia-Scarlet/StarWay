#include "TinyEngine\precomp.h"
#include "BaseComponent.h"
#include "TinyEngine\Objects\Object.h"


BaseComponent::BaseComponent()
	:_id(genericObjectId())
{
}


BaseComponent::~BaseComponent()
{
}

void BaseComponent::setOwner(const RefCountPtr<Object> & owner)
{
	_owner = owner;
}

WeakRefPtr<Object> BaseComponent::getOwner()
{
	return _owner;
}
