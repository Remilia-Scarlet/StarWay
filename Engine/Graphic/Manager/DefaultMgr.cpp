#include "Graphic/precomp.h"
#include "DefaultMgr.h"
#include "ShaderMgr.h"

DefaultMgr* DefaultMgr::s_instance = nullptr;

bool DefaultMgr::createInstance()
{
	DefaultMgr* instan = new DefaultMgr();
	if (!instan || !instan->init())
	{
		TinyAssert(false, "Create DefaultMgr failed!");
		if (instan)
			delete instan;
		return false;
	}
	s_instance = instan;
	return true;
}

DefaultMgr* DefaultMgr::instance()
{
	TinyAssert(s_instance);
	return s_instance;
}

void DefaultMgr::destroyInstance()
{
	TINY_SAFE_DELETE(s_instance);
}

const GfxTexturePtr& DefaultMgr::getDefaultTexture()
{
	return _defaultTexture;
}

const GfxMaterialPtr& DefaultMgr::getDefaultMaterial()
{
	return _defaultMaterial;
}

const GfxShaderPixelPtr& DefaultMgr::getDefaultPSWithTex()
{
	return _defaultPSWithTex;
}

const GfxShaderPixelPtr& DefaultMgr::getDefaultPSWithNoTex()
{
	return _defaultPSWithNoTex;
}

const GfxShaderVertexPtr& DefaultMgr::getDefaultVS()
{
	return _defaultVS;
}

bool DefaultMgr::init()
{
	do 
	{
		_defaultTexture = GfxTexture::create();
		TINY_BREAK_IF(!_defaultTexture.isValid());
		_defaultMaterial = GfxMaterial::create({ 0.48f, 0.77f, 0.46f, 1.0f }, { 0.48f, 0.77f, 0.46f, 1.0f }, { 0.2f, 0.2f, 0.2f, 16.0f });
		TINY_BREAK_IF(!_defaultMaterial.isValid());
		_defaultPSWithTex = ShaderMgr::instance()->getPSShader("DefaultShader_PS.cso");
		TINY_BREAK_IF(!_defaultPSWithTex.isValid());
		_defaultPSWithNoTex = ShaderMgr::instance()->getPSShader("DefaultShader_NoTex_PS.cso");
		TINY_BREAK_IF(!_defaultPSWithNoTex.isValid());
		_defaultVS = ShaderMgr::instance()->getVSShader("DefaultShader_VS.cso");
		TINY_BREAK_IF(!_defaultVS.isValid());
		return true;
	} while (0);
	TinyAssert(false, "DefaultMgr create failed");
	return false;
}

