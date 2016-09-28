#pragma once
#include "Graphic/Manager/GraphicMgr.h"
class GfxIndexBuffer;
TINY_DEFINE_PTR(GfxShaderPixel);
class DX11GraphicMgr :	public GraphicMgr
{
	friend class GraphicMgr;
public:
	virtual void setViewPort(const ViewPort& viewPort)override;
	virtual ViewPort getViewPort()override;

	virtual void preRender() override;
	virtual void draw() override;
	virtual void render()override;

	virtual void setPixelShader(const GfxShaderPixelPtr& ps)override;
	virtual void setVertexShader(const GfxShaderVertexPtr& vs)override;

	virtual void setInputLayout(InputLayoutType inputLayoutType) override;
	
	ID3D11DeviceContext* getD3D11DeviceContext() { return _pImmediateContext; }
	ID3D11Device* getD3D11Device() { return _pd3dDevice; }
protected:
	DX11GraphicMgr();
	virtual ~DX11GraphicMgr();

	D3D_DRIVER_TYPE                     _driverType = D3D_DRIVER_TYPE_NULL;
	IDXGISwapChain*                     _pSwapChain = NULL;
	ID3D11Device*                       _pd3dDevice = NULL;
	D3D_FEATURE_LEVEL                   _featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11DeviceContext*                _pImmediateContext = NULL;
	ID3D11RenderTargetView*             _pRenderTargetView = NULL;
	ID3D11Texture2D*                    _pDepthStencil = NULL;
	ID3D11DepthStencilView*             _pDepthStencilView = NULL;
	ID3D11InputLayout*					_pInputLayout[static_cast<size_t>(InputLayoutType::TYPE_NUMBER)];

	static const float s_clearColor[4]; //use this color to clear screen
	virtual bool init(int width, int height, HWND hWnd);
	bool initInputLayout();
	bool initDevice(int width, int height, HWND hWnd);
	bool initDepthStencil(int width, int height);
	virtual void clearDevice() override;
};

