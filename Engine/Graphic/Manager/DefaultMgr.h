#pragma once
#include "Graphic/gfx/GfxShaderVertex.h"
#include "Graphic/gfx/GfxShaderPixel.h"
#include "Graphic/gfx/GfxTexture.h"

class DefaultMgr
{
public:
	static bool createInstance();
	static DefaultMgr* instance();
	static void destroyInstance();

	const GfxTexturePtr& getDefaultTexture();

	const GfxShaderPixelPtr& getDefaultPSWithTex();
	const GfxShaderPixelPtr& getDefaultPSWithNoTex();
	const GfxShaderVertexPtr& getDefaultVS();
protected:
	bool init();
	GfxTexturePtr _defaultTexture; //texture with no picture

	GfxShaderPixelPtr _defaultPSWithTex;
	GfxShaderPixelPtr _defaultPSWithNoTex;
	GfxShaderVertexPtr _defaultVS;

	static DefaultMgr* s_instance;
};