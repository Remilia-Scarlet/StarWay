#pragma once
#include "BaseComponent.h"
#include <functional>

TINY_DEFINE_PTR(ScriptComponent);
class ScriptComponent : public BaseComponent
{
public:
	static ScriptComponentPtr create(const std::function<void()>& callback);

	void setOwner(const RefCountPtr<Object> & owner) override;

	virtual void update(float dt) override;
protected:
	virtual bool init(const std::function<void()>& callback);
	ScriptComponent();
	virtual ~ScriptComponent();

	std::function<void()> _callback;
	bool _inited;
};

