#pragma once
#include "Graphic/GfxDefines.h"
#include "Ash/RefCountPointer/RefCountObj.h"
#include "TinyEngine/Engine/EngineDefs.h"
TINY_DEFINE_PTR(GfxShaderVertex);

class GfxShaderVertex : public RefCountObj
{
private:
	GfxShaderVertex();
	virtual ~GfxShaderVertex();
public:
	static GfxShaderVertexPtr create(const std::string& shaderFileName);
	static GfxShaderVertexPtr create(CompiledVSShader* shader);
public:
	CompiledVSShader* getPlatformVSShader() { return _compiledVSShader; }

	void render();
protected:
	virtual bool init(const std::string& shaderFileName);
	virtual bool init(CompiledVSShader* shader);
	bool createVSShaderFromFile(const std::string& filename);
	CompiledVSShader* _compiledVSShader;
};
