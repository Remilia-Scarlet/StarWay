#pragma once
#include "BaseComponent.h"
#include "TinyEngine/ScriptManager/LuaManager.h"
#include "Math/vector/Vector.h"
#include "Graphic/Manager/LightManager.h"

TINY_DEFINE_PTR(DirectionLightComponent);
class DirectionLightComponent : public BaseComponent
{
public:
	static bool createLuaPrototype();
public:
	static DirectionLightComponentPtr create(const Vector4& ambient,const Vector4& diffuse,const Vector4& specular,const Vector3& direction);
	LUA_CREATE_FUN_P4(DirectionLightComponent, const Vector4&, const Vector4&, const Vector4&, const Vector3&);

	virtual void render() override;
protected:
	bool init(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, const Vector3& direction);

	DirectionLightComponent();
	virtual ~DirectionLightComponent();
	DirectionLight _light;
};

