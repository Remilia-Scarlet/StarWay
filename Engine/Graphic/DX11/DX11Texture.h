#pragma once
#include "Graphic\gfx\GfxTexture.h"
#include <d3d11.h>

TINY_DEFINE_PTR(DX11Texture);
class DX11Texture : public GfxTexture
{
	friend class GfxTexture;
public:
	virtual void render() override;
public:

protected:
	virtual bool init(const uint8_t* ddsData, int dataSize, const char* debugName) override;
	ID3D11Resource* _texture;
	ID3D11ShaderResourceView* _textureView;
	ID3D11SamplerState* _samplerState;
protected:
	DX11Texture();
	virtual ~DX11Texture();
};

