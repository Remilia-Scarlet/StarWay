#pragma once
#include "BaseComponent.h"
#include "Graphic\gfx\GfxTexture.h"
#include "Graphic\DX11\DX11GraphicMgr.h"

TINY_DEFINE_PTR(TextureComponent);
class TextureComponent : public BaseComponent
{
public:
	virtual ~TextureComponent();
public:
	const static TextureComponentPtr DefaultTexture;

	static TextureComponentPtr create(const std::string& fileName, const std::string& shadeName);

	GfxTexturePtr getGfxTexture() { return _gfxTexture; }

	virtual void render() override;
protected:
	virtual bool init(const std::string& fileName, const std::string& shadeName);
	explicit TextureComponent();

	GfxTexturePtr _gfxTexture;
	GfxShaderPixelPtr _psShader;
};

