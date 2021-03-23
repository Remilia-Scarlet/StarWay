#pragma once
#include "BaseComponent.h"
#include "DangoMath/vector/Vector.h"
#include "Graphic/Manager/LightManager.h"

ASH_DEFINE_PTR(DirectionLightComponent);
class DirectionLightComponent : public BaseComponent
{
public:
	static DirectionLightComponentPtr create(
		const DangoMath::Vector4& ambient,//xyz:color w:intensity
		const DangoMath::Vector4& diffuse,//xyz:color w:intensity
		const DangoMath::Vector4& specular);//xyz:color w:intensity

	virtual void render() override;
	bool prepareRenderShadowMap();
	void finishedRenderShadowMap();

	const GfxRenderBufferPtr& getShadowMapBuffer();
protected:
	bool init(const DangoMath::Vector4& ambient, const DangoMath::Vector4& diffuse, const DangoMath::Vector4& specular);
	virtual void setOwner(const Ash::RefCountPtr<Object> & owner) override;
	DirectionLightComponent();
	virtual ~DirectionLightComponent();
	DirectionLight _light;
	GfxRenderBufferPtr _shadowMapBuffer;
};

