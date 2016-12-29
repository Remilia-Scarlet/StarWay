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
	static DirectionLightComponentPtr create(
		const Vector4& ambient,//xyz:color w:intensity
		const Vector4& diffuse,//xyz:color w:intensity
		const Vector4& specular);//xyz:color w:intensity
	LUA_CREATE_FUN_P3(DirectionLightComponent, const Vector4&, const Vector4&, const Vector4&);

	virtual void render() override;
	bool prepareRenderShadowMap();
	void finishedRenderShadowMap();

	const GfxRenderBufferPtr& getShadowMapBuffer();
protected:
	bool init(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular);
	virtual void setOwner(const RefCountPtr<Object> & owner) override;
	DirectionLightComponent();
	virtual ~DirectionLightComponent();
	DirectionLight _light;
	GfxRenderBufferPtr _shadowMapBuffer;
};

