#include "TinyEngine\precomp.h"
#include "DirectionLightComponent.h"


bool DirectionLightComponent::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(create);

	LUA_PROTOTYPE_END(DirectionLightComponent);
	return true;
}

DirectionLightComponentPtr DirectionLightComponent::create(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, const Vector3& direction)
{
	DirectionLightComponent* ret = new DirectionLightComponent();
	if (ret && ret->init(ambient, diffuse, specular, direction))
		return DirectionLightComponentPtr(ret);
	TINY_SAFE_DELETE(ret);
	return DirectionLightComponentPtr();
}

void DirectionLightComponent::render()
{
	LightManager::instance()->applyDirectionLight(_light);
}

DirectionLightComponent::DirectionLightComponent()
	:BaseComponent(TO_STRING(DirectionLightComponent))
{
}

DirectionLightComponent::~DirectionLightComponent()
{
}

bool DirectionLightComponent::init(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, const Vector3& direction)
{
	do
	{
		_light.ambient = ambient;
		_light.diffuse = diffuse;
		_light.specular = specular;
		_light.direction = direction.normalized();
		return true;
	} while (0);
	return false;
}