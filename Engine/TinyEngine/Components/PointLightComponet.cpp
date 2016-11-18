#include "TinyEngine\precomp.h"
#include "PointLightComponet.h"


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
	const Vector3& position,
	float range,
	const Vector3& att)
{
	PointLightComponet* ret = new PointLightComponet();
	if (ret && ret->init(ambient, diffuse, specular, position, range, att))
		return PointLightComponetPtr(ret);
	TINY_SAFE_DELETE(ret);
	return PointLightComponetPtr();
}

int PointLightComponet::L_create(lua_State* L)
{
	Vector4 ambient = (Vector4)LuaManager::instance()->getVal(L, 2);
	Vector4 diffuse = (Vector4)LuaManager::instance()->getVal(L, 3);
	Vector4 specular = (Vector4)LuaManager::instance()->getVal(L, 4);
	Vector3 position = (Vector3)LuaManager::instance()->getVal(L, 5);
	float range = LuaManager::instance()->getVal(L, 6).convertFloat();
	Vector3 att = (Vector3)LuaManager::instance()->getVal(L, 7);

}

void PointLightComponet::render()
{
	LightManager::instance()->applyPointLight(_light);
}

PointLightComponet::PointLightComponet()
	:BaseComponent(TO_STRING(PointLightComponet))
{
}

PointLightComponet::~PointLightComponet()
{
}

bool PointLightComponet::init(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, const Vector3& position, float range, const Vector3& att)
{
	do
	{
		_light.ambient = ambient;
		_light.diffuse = diffuse;
		_light.specular = specular;
		_light.position = position;
		_light.range = range;
		_light.att = att;
		return true;
	} while (0);
	return false;
}