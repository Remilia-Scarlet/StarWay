#include "TinyEngine\precomp.h"
#include "LightComponent.h"


LightComponent::LightComponent()
	:BaseComponent(TO_STRING(LightComponent))
{
}


bool LightComponent::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_END(LightComponent);
	return true;
}

LightComponent::~LightComponent()
{
}
