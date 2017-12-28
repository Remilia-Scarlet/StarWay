#include "precomp.h"
#include "PointLightComponet.h"
#include "TransformComponent.h"


bool PointLightComponet::createLuaPrototype()
{
	LUA_PROTOTYPE_PREPARE();

	LUA_PROTOTYPE_REGIST_FUN(create);

	LUA_PROTOTYPE_END(PointLightComponet);
	return true;
}

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

int PointLightComponet::L_create(lua_State* L)
{
	Vector4 ambient = LuaManager::instance()->getVal<Vector4>(L, 2);
	Vector4 diffuse = LuaManager::instance()->getVal<Vector4>(L, 3);
	Vector4 specular = LuaManager::instance()->getVal<Vector4>(L, 4);
	float range = LuaManager::instance()->getVal<float>(L, 5);
	Vector3 att = LuaManager::instance()->getVal<Vector3>(L, 6);
	PointLightComponetPtr ptr = PointLightComponet::create(ambient, diffuse, specular, range, att);
	LuaManager::instance()->pushVal(ptr);
	return 1;
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
