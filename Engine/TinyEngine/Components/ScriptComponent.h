#pragma once
#include "BaseComponent.h"
#include "TinyEngine/ScriptManager/LuaManager.h"

TINY_DEFINE_PTR(ScriptComponent);
class ScriptComponent : public BaseComponent
{
public:
	static void createLuaPrototype();
public:
	static ScriptComponentPtr create(const std::string& luaFunctionTable);
	LUA_CREATE_FUN_P1(ScriptComponent, const std::string&);

	void setOwner(const RefCountPtr<Object> & owner) override;

	virtual void update(float dt) override;
protected:
	virtual bool init(const std::string& luaFunctionTable);
	ScriptComponent();
	virtual ~ScriptComponent();

	std::string _luaFunctionTable;
	bool _inited;
};

