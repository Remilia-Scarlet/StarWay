#pragma once
#include "Graphic\gfx\GfxShaderVertex.h"
#include "Graphic\gfx\GfxShaderPixel.h"
class ShaderMgr
{
public:
	static bool createInstance();
	static void destroyInstance();
	static ShaderMgr* instance();
public:
	GfxShaderVertexPtr getVSShader(const std::string& filename);
	GfxShaderVertexPtr getDefaultVSShader() { return _defaultVShader; }

	GfxShaderPixelPtr getPSShader(const std::string& filename);
	GfxShaderPixelPtr getDefaultPSShader() { return _defaultPShader; }
protected:
	ShaderMgr();
	virtual ~ShaderMgr();
	static ShaderMgr* s_instance;
	GfxShaderVertexPtr _defaultVShader;
	GfxShaderPixelPtr _defaultPShader;

	virtual bool init();
	std::unordered_map<std::string, GfxShaderVertexPtr> _vsShaders;
	std::unordered_map<std::string, GfxShaderPixelPtr> _psShaders;
};

