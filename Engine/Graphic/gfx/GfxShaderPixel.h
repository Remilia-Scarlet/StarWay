#pragma once
#include "Graphic/GfxDefines.h"
#include "Ash/RefCountPointer/RefCountObj.h"
TINY_DEFINE_PTR(GfxShaderPixel);
class GfxShaderPixel : public RefCountObj
{
private:
	GfxShaderPixel();
	virtual ~GfxShaderPixel();
public:
	static GfxShaderPixelPtr create(const std::string& shaderFileName);
	static GfxShaderPixelPtr create(ShaderBlob* blob, CompiledPSShader* shader);
public:
	CompiledPSShader* getPlatformPSShader() { return _compiledPSShader; }

	void render();
protected:
	virtual bool init(const std::string& shaderFileName);
	virtual bool init(ShaderBlob* blob, CompiledPSShader* shader);
	bool createPSShaderFromFile(const std::string& filename);
	CompiledPSShader* _compiledPSShader;
};
