#pragma once
#include "BaseComponent.h"

TINY_DEFINE_PTR(ScriptComponent);
class ScriptComponent : public BaseComponent
{
public:
	static ScriptComponentPtr create();

protected:
	virtual bool init();
	ScriptComponent();
	virtual ~ScriptComponent();

};

