#include "TinyEngine/precomp.h"
#include "ScriptComponent.h"
#include "TinyEngine\Objects\Object.h"

ScriptComponentPtr ScriptComponent::create(std::shared_ptr<Script> script)
{
	ScriptComponent* ret = new ScriptComponent();
	if (ret && ret->init(script))
		return ScriptComponentPtr(ret);
	TINY_SAFE_DELETE(ret);
	return ScriptComponentPtr();
}

void ScriptComponent::setOwner(const RefCountPtr<Object> & owner)
{
	BaseComponent::setOwner(owner);
	_script->_owner = owner;
}

void ScriptComponent::update(Task* task, float dt)
{
	BaseComponent::update(task, dt);
	if (!_inited)
	{
		_script->init();
		_inited = true;
	}
	_script->update(dt);
}

bool ScriptComponent::init(std::shared_ptr<Script>& script)
{
	do 
	{
		TINY_BREAK_IF(!script);
		_script = std::move(script);
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
