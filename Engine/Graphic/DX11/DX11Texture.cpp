#include "precomp.h"
#include "DX11Texture.h"
#include "DDSTextureLoader.h"
#include "Graphic\Manager\GraphicMgr.h"

bool DX11Texture::init(const uint8_t* ddsData, int dataSize, const char* debugName)
{
	do
	{
		if (ddsData && dataSize)
		{
			HRESULT hr = S_OK;
			hr = DirectX::CreateDDSTextureFromMemory(GraphicMgr::instance()->getD3D11Device(), ddsData, (size_t)dataSize, &_texture, &_textureView);
			TINY_BREAK_IF(FAILED(hr));
			SET_DEBUG_NAME(_texture, FormatString("Texture_%s", debugName).c_str());
			SET_DEBUG_NAME(_textureView, FormatString("TextureView_%s", debugName).c_str());
			// Create the sample state
			D3D11_SAMPLER_DESC sampDesc;
			ZeroMemory(&sampDesc, sizeof(sampDesc));
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
			hr = GraphicMgr::instance()->getD3D11Device()->CreateSamplerState(&sampDesc, &_samplerState);
			TINY_BREAK_IF(FAILED(hr));
			SET_DEBUG_NAME(_samplerState, FormatString("SamplerState_%s", debugName).c_str());
		}
		return true;
	} while (0);
	DebugString("Failed to create DX11Texture %s", debugName);
	return false;
}

DX11Texture::DX11Texture()
	: _texture(nullptr)
	, _samplerState(nullptr)
	, _textureView(nullptr)
	, _wireFrame(false)
{
}


DX11Texture::~DX11Texture()
{
	TINY_SAFE_RELEASE(_texture);
	TINY_SAFE_RELEASE(_textureView);
	TINY_SAFE_RELEASE(_samplerState);
}

void DX11Texture::render()
{
	GraphicMgr::instance()->setWireFrame(_wireFrame);

	if (_textureView && _samplerState)
	{
		GraphicMgr::instance()->getD3D11DeviceContext()->PSSetShaderResources(0, 1, &_textureView);
		GraphicMgr::instance()->getD3D11DeviceContext()->PSSetSamplers(0, 1, &_samplerState);
	}
}

bool DX11Texture::isWireFrame()
{
	return _wireFrame;
}

void DX11Texture::setWireFrame(bool wireFrame)
{
	_wireFrame = wireFrame;
}
