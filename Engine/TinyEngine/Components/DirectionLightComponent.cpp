#include "TinyEngine\precomp.h"
#include "DirectionLightComponent.h"
#include "TransformComponent.h"


bool DirectionLightComponent::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(create);

	LUA_PROTOTYPE_END(DirectionLightComponent);
	return true;
}

DirectionLightComponentPtr DirectionLightComponent::create(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular)
{
	DirectionLightComponent* ret = new DirectionLightComponent();
	if (ret && ret->init(ambient, diffuse, specular))
		return DirectionLightComponentPtr(ret);
	TINY_SAFE_DELETE(ret);
	return DirectionLightComponentPtr();
}

void DirectionLightComponent::render()
{
	if (!_owner.isValid())
		return;
	TransformComponentPtr trans = _owner.lock()->getComponent<TransformComponent>();
	if (!trans.isValid())
		return;
	_light.direction = Vector3(0,0,1).rotateInPlace(trans->getRotation());
	LightManager::instance()->applyDirectionLight(_light);
}

DirectionLightComponent::DirectionLightComponent()
	:BaseComponent(TO_STRING(DirectionLightComponent))
{
}

DirectionLightComponent::~DirectionLightComponent()
{
}

bool DirectionLightComponent::init(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular)
{
	do
	{
		_light.ambient = ambient;
		_light.diffuse = diffuse;
		_light.specular = specular;
		return true;
	} while (0);
	return false;
}