#pragma once
#include "BaseComponent.h"
#include "Graphic\gfx\GfxTexture.h"
#include "Graphic\DX11\DX11GraphicMgr.h"
#include "TinyEngine\ScriptManager\LuaManager.h"
#include "Graphic\gfx\GfxMaterial.h"

TINY_DEFINE_PTR(TextureComponent);
class TextureComponent : public BaseComponent
{
public:
	static bool createLuaPrototype();
public:
	virtual ~TextureComponent();
public:
	const static TextureComponentPtr DefaultTexture;

	static TextureComponentPtr create();
	static TextureComponentPtr create(const std::string& fileName);
	static TextureComponentPtr create(const std::string& fileName, const std::string& shadeName);
	static int L_create(lua_State* L);

	void setTexture(const GfxTexturePtr& texture);
	const GfxTexturePtr& getTexture();

	void setMaterial(const GfxMaterialPtr& material);
	const GfxMaterialPtr& getMaterial();

	virtual void render() override;
protected:
	virtual bool init(const std::string& fileName, const std::string& shadeName);
	TextureComponent();

	GfxTexturePtr _gfxTexture;
	GfxMaterialPtr _gfxMaterial;

	GfxShaderPixelPtr _psShader;
};

