#include "precomp.h"
#include "ScriptComponent.h"
#include "TinyEngine\Objects\Object.h"

ScriptComponentPtr ScriptComponent::create(const std::function<void()>& callback)
{
	ScriptComponent* ret = new ScriptComponent();
	if (ret && ret->init(callback))
		return ScriptComponentPtr(ret);
	TINY_SAFE_DELETE(ret);
	return ScriptComponentPtr();
}

void ScriptComponent::setOwner(const RefCountPtr<Object> & owner)
{
	BaseComponent::setOwner(owner);
}

void ScriptComponent::update(float dt)
{
	BaseComponent::update(dt);
}

bool ScriptComponent::init(const std::function<void()>& callback)
{
	do 
	{
		_callback = callback;
		return true;
	} while (false);
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
