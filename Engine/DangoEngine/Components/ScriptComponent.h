#pragma once
#include "BaseComponent.h"
#include <functional>

class Script
{
public:
	virtual ~Script() = default;
	virtual void init(){};
	virtual void update(float dt){};
	WeakRefPtr<Object> _owner;
};

ASH_DEFINE_PTR(ScriptComponent);
class ScriptComponent : public BaseComponent
{
public:
	static ScriptComponentPtr create(std::shared_ptr<Script> script);

	void setOwner(const RefCountPtr<Object> & owner) override;

	virtual void update(Task* task, float dt) override;
protected:
	virtual bool init(std::shared_ptr<Script>& script);
	ScriptComponent();
	virtual ~ScriptComponent();

	std::shared_ptr<Script> _script;
	bool _inited;
};

