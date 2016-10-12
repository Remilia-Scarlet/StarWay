#include "Graphic/precomp.h"
#include "ShaderMgr.h"
#include "GraphicMgr.h"

ShaderMgr* ShaderMgr::s_instance = nullptr;

bool ShaderMgr::createInstance()
{
	TinyAssert(s_instance == nullptr, "Memory leaks! Call destroyInstance() first!");
	s_instance = new ShaderMgr();
	if (!s_instance || !s_instance->init())
		TINY_SAFE_DELETE(s_instance);
	return s_instance != nullptr;
}

void ShaderMgr::destroyInstance()
{
	TINY_SAFE_DELETE(s_instance);
}

ShaderMgr* ShaderMgr::instance()
{
	TinyAssert(s_instance != nullptr, "No instance!! Call createInstance() first!");
	return s_instance;
}

GfxShaderVertexPtr ShaderMgr::getVSShader(const std::string& filename)
{
	GfxShaderVertexPtr& shader = _vsShaders[filename];
	if (!shader.isValid())
	{
		GfxShaderVertexPtr p = GfxShaderVertex::create(filename);
		if (p.isValid())
			shader = p;
	}
	return shader;
}

GfxShaderPixelPtr ShaderMgr::getPSShader(const std::string& filename)
{
	GfxShaderPixelPtr& shader = _psShaders[filename];
	if (!shader.isValid())
	{
		GfxShaderPixelPtr p = GfxShaderPixel::create(filename);
		if (p.isValid())
			shader = p;
	}
	return shader;
}

ShaderMgr::ShaderMgr()
{

}

ShaderMgr::~ShaderMgr()
{
	if (_defaultVShader.isValid())
		_defaultVShader = nullptr;
	if (_defaultPShader.isValid())
		_defaultPShader = nullptr;
}

bool ShaderMgr::init()
{
	do 
	{
		_defaultVShader = getVSShader("default_shader_VS.cso");
		TINY_BREAK_IF(!_defaultVShader.isValid());
		_defaultPShader = getPSShader("default_shader_PS.cso");
		TINY_BREAK_IF(!_defaultPShader.isValid());
		return true;
	} while (0);
	TinyAssert(false, "ShaderMgr creating failed!");
	return false;
}
