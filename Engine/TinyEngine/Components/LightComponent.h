#pragma once
#include "BaseComponent.h"
#include "TinyEngine/ScriptManager/LuaManager.h"

TINY_DEFINE_PTR(LightComponent);
class LightComponent : public BaseComponent
{
public:
	static bool createLuaPrototype();
public:
	virtual ~LightComponent();
public:

protected:
	LightComponent();

};

