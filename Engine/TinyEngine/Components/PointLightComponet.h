#pragma once
#include "BaseComponent.h"
#include "Math/vector/Vector.h"
#include "Graphic/Manager/LightManager.h"

TINY_DEFINE_PTR(PointLightComponet);
class PointLightComponet : public BaseComponent
{
public:
	static PointLightComponetPtr create(const Vector4& ambient,
										const Vector4& diffuse,
										const Vector4& specular,
										float range,
										const Vector3& att);

	virtual void render() override;
protected:
	virtual bool init(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, float range, const Vector3& att);
	virtual void setOwner(const RefCountPtr<Object> & owner) override;
	PointLightComponet();
	virtual ~PointLightComponet();
	PointLight _light;
};

