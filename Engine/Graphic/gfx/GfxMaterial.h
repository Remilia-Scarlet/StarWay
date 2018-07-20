#pragma once
#include "Graphic/GfxDefines.h"
#include "Ash/RefCountPointer/RefCountObj.h"
#include "Math/vector/Vector.h"
#include "GfxShaderPixel.h"

TINY_DEFINE_PTR(GfxMaterial);
class GfxMaterial : public RefCountObj
{
protected:
	GfxMaterial();
	virtual ~GfxMaterial();

public:
	static GfxMaterialPtr create();
	static GfxMaterialPtr create(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, const Vector4& emit);

	const Vector4& getAmbient();
	void setAmbient(const Vector4& ambient);

	const Vector4& getDiffuse();
	void setDiffuse(const Vector4& diffuse);

	const Vector4& getSpecular();
	void setSpecular(const Vector4& specular);

	const Vector4& getEmit();
	void setEmit(const Vector4& emit);

	void render();
private:
	virtual bool init(const Vector4& ambient, const Vector4& diffuse, const Vector4& specular, const Vector4& emit);
	Vector4 _ambient;
	Vector4 _diffuse;
	Vector4 _specular;// w is weight
	Vector4 _emit;
	GfxShaderPixel _shader;
};


