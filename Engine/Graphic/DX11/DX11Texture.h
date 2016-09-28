#pragma once
#include "Graphic\gfx\GfxTexture.h"
#include <d3d11.h>

TINY_DEFINE_PTR(DX11Texture);
class DX11Texture : public GfxTexture
{
public:
	DX11Texture();
	virtual ~DX11Texture();

	virtual void render() override;
protected:
	virtual bool init(const uint8_t* ddsData, int dataSize, const char* debugName) override;
	ID3D11Resource* _texture;
	ID3D11ShaderResourceView* _textureView;
	ID3D11SamplerState* _samplerState;
};

