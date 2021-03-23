#include "DangoEngine/precomp.h"
#include "PointLightComponet.h"
#include "TransformComponent.h"

PointLightComponetPtr PointLightComponet::create(
	const Vector4& ambient,
	const Vector4& diffuse,
	const Vector4& specular,
	float range,
	const Vector3& att)
{
	PointLightComponet* ret = new PointLightComponet();
	if (ret && ret->init(ambient, diffuse, specular, range, att))
		return PointLightComponetPtr(ret);
	TINY_SAFE_DELETE(ret);
	return PointLightComponetPtr();
}

void PointLightComponet::render()
{
	ObjectPtr obj = getOwner().lock();
	TinyAssert(obj.isValid());
	TransformComponentPtr trans = obj->getComponent<TransformComponent>();
	if (!trans.isValid())
		return;
	_light.position = trans->getLocation();
	LightManager::instance()->applyPointLight(_light);
}

PointLightComponet::PointLightComponet()
	:BaseComponent(TO_STRING(PointLightComponet))
{
}

PointLightComponet::~PointLightComponet()
{
}

bool PointLightComponet::init(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, float range, const Vector3& att)
{
	do
	{
		_light.ambient = ambient;
		_light.diffuse = diffuse;
		_light.specular = specular;
		_light.range = range;
		_light.att = att;
		return true;
	} while (0);
	return false;
}

void PointLightComponet::setOwner(const RefCountPtr<Object> & owner)
{
	BaseComponent::setOwner(owner);
	if (owner.isValid())
	{
		owner->setFlag(ObjectFlag::IS_LIGHT, true);
		owner->setFlag(ObjectFlag::IS_POINT_LIGHT, true);
	}
}
