#include "Graphic/precomp.h"
#include "DX11RenderBuffer.h"
#include "Graphic\Manager\GraphicMgr.h"

void DX11RenderBuffer::render()
{

}

bool DX11RenderBuffer::init(int width, int height)
{
	do 
	{
		HRESULT hr = S_OK;
		// Create depth stencil texture
		D3D11_TEXTURE2D_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Width = width;
		desc.Height = height;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;
		hr = GraphicMgr::instance()->getD3D11Device()->CreateTexture2D(&desc, NULL, &_textureBuffer);
		if (FAILED(hr))
			return false;
		SET_DEBUG_NAME(_textureBuffer, FormatString("DepthStencil_w%d_h%d", width, height).c_str());

		// Create the depth stencil view
		D3D11_RENDER_TARGET_VIEW_DESC descDSV;
		ZeroMemory(&descDSV, sizeof(descDSV));
		descDSV.Format = desc.Format;
		descDSV.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = GraphicMgr::instance()->getD3D11Device()->CreateRenderTargetView(_textureBuffer, &descDSV, &_renderTargetView);
		if (FAILED(hr))
			return false;
		SET_DEBUG_NAME(_renderTargetView, FormatString("RenderTargetView_w%d_h%d", width, height).c_str());

		// Create the shader resource view.
		D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
		shaderResourceViewDesc.Format = desc.Format;
		shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
		shaderResourceViewDesc.Texture2D.MipLevels = 1;
		hr = GraphicMgr::instance()->getD3D11Device()->CreateShaderResourceView(_textureBuffer, &shaderResourceViewDesc, &_shaderResourceView);
		if (FAILED(hr))
			return false;

		return true;
	} while (false);
	return false;
}

ID3D11RenderTargetView* DX11RenderBuffer::getRenderTargetView()
{
	return _renderTargetView;
}

ID3D11ShaderResourceView* DX11RenderBuffer::getShaderResourceView()
{
	return _shaderResourceView;
}

DX11RenderBuffer::DX11RenderBuffer()
{

}

DX11RenderBuffer::~DX11RenderBuffer()
{
	TINY_SAFE_RELEASE(_renderTargetView);
	TINY_SAFE_RELEASE(_textureBuffer);
	TINY_SAFE_RELEASE(_shaderResourceView);
}
