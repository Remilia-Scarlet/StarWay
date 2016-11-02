#pragma once
#include "BaseComponent.h"

TINY_DEFINE_PTR(ScriptComponent);
class ScriptComponent : public BaseComponent
{
public:
	static ScriptComponentPtr create(const std::string& luaFunctionTable);
	virtual void update(float dt) override;
protected:
	virtual bool init(const std::string& luaFunctionTable);
	ScriptComponent();
	virtual ~ScriptComponent();

	std::string _luaFunctionTable;
};

