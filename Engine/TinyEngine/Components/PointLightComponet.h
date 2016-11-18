#pragma once
#include "BaseComponent.h"
#include "TinyEngine/ScriptManager/LuaManager.h"
#include "Math/vector/Vector.h"
#include "Graphic/Manager/LightManager.h"

TINY_DEFINE_PTR(PointLightComponet);
class PointLightComponet : public BaseComponent
{
public:
	static bool createLuaPrototype();
public:
	static PointLightComponetPtr create(const Vector4& ambient,
										const Vector4& diffuse,
										const Vector4& specular,

										const Vector3& position,
										float range,

										const Vector3& att);
	static int L_create(lua_State* L);

	virtual void render() override;
protected:
	virtual bool init(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, const Vector3& position, float range, const Vector3& att);

	PointLightComponet();
	virtual ~PointLightComponet();
	PointLight _light;
};

