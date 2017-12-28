#include "precomp.h"
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
	if (filename.empty())
		return nullptr;

	GfxShaderVertexPtr& shader = _vsShaders[filename];
	if (!shader.isValid())
	{
		shader = GfxShaderVertex::create(filename);
	}
	return shader;
}

GfxShaderPixelPtr ShaderMgr::getPSShader(const std::string& filename)
{
	if (filename.empty())
		return nullptr;

	GfxShaderPixelPtr& shader = _psShaders[filename];
	if (!shader.isValid())
	{
		shader = GfxShaderPixel::create(filename);
	}
	return shader;
}

ShaderMgr::ShaderMgr()
{

}

ShaderMgr::~ShaderMgr()
{
}

bool ShaderMgr::init()
{
	do 
	{
		return true;
	} while (0);
	TinyAssert(false, "ShaderMgr creating failed!");
	return false;
}
