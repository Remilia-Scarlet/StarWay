#pragma once
#include "Graphic/gfx/GfxShaderVertex.h"
#include "Graphic/gfx/GfxShaderPixel.h"
#include "Graphic/gfx/GfxTexture.h"
#include "Graphic/gfx/GfxMaterial.h"

class DefaultMgr
{
public:
	static bool createInstance();
	static DefaultMgr* instance();
	static void destroyInstance();

	const GfxTexturePtr& getDefaultTexture();
	const GfxMaterialPtr& getDefaultMaterial();
	const GfxShaderPixelPtr& getDefaultPSWithTex();
	const GfxShaderPixelPtr& getDefaultPSWithNoTex();
	const GfxShaderVertexPtr& getDefaultVS();
protected:
	bool init();
	GfxTexturePtr _defaultTexture; //texture with no picture
	GfxMaterialPtr _defaultMaterial;
	GfxShaderPixelPtr _defaultPSWithTex;
	GfxShaderPixelPtr _defaultPSWithNoTex;
	GfxShaderVertexPtr _defaultVS;

	static DefaultMgr* s_instance;
};