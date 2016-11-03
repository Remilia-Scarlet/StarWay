#pragma once
#include "BaseComponent.h"
#include "Graphic\gfx\GfxTexture.h"
#include "Graphic\DX11\DX11GraphicMgr.h"
#include "TinyEngine/ScriptManager/LuaManager.h"

TINY_DEFINE_PTR(TextureComponent);
class TextureComponent : public BaseComponent
{
public:
	static bool createLuaPrototype();
public:
	virtual ~TextureComponent();
public:
	const static TextureComponentPtr DefaultTexture;

	static TextureComponentPtr create(const std::string& fileName, const std::string& shadeName);
	LUA_CREATE_FUN_P2(TextureComponent,const std::string&, const std::string&);

	GfxTexturePtr getGfxTexture() { return _gfxTexture; }

	virtual void render() override;
protected:
	virtual bool init(const std::string& fileName, const std::string& shadeName);
	TextureComponent();

	GfxTexturePtr _gfxTexture;
	GfxShaderPixelPtr _psShader;
};

