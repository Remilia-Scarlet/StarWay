#pragma once
#include "Graphic\gfx\GfxRenderBuffer.h"
#include <d3d11.h>

TINY_DEFINE_PTR(DX11RenderBuffer);
class DX11RenderBuffer : public GfxRenderBuffer
{
	friend class GfxRenderBuffer;
public:
	virtual void render() override;

	ID3D11RenderTargetView* getRenderTargetView();
	ID3D11ShaderResourceView* getShaderResourceView();
protected:
	virtual bool init(int width, int height) override;
protected:
	DX11RenderBuffer();
	virtual ~DX11RenderBuffer();

	ID3D11RenderTargetView*             _renderTargetView = NULL;
	ID3D11Texture2D*                    _textureBuffer = NULL;
	ID3D11ShaderResourceView*			_shaderResourceView = NULL;
};

