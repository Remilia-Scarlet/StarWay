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
	
	ID3D11DeviceContext* getD3D11DeviceContext() { return _immediateContext; }
	ID3D11Device* getD3D11Device() { return _d3dDevice; }

	void setWireFrame(bool isWireFrame);

	virtual void setRenderBuffer(const GfxRenderBufferPtr& buffer) override;
	virtual void resetRenderBuffer() override;
protected:
	DX11GraphicMgr();
	virtual ~DX11GraphicMgr();

	D3D_DRIVER_TYPE                     _driverType = D3D_DRIVER_TYPE_NULL;
	IDXGISwapChain*                     _swapChain = NULL;
	ID3D11Device*                       _d3dDevice = NULL;
	D3D_FEATURE_LEVEL                   _featureLevel = D3D_FEATURE_LEVEL_11_0;
	ID3D11DeviceContext*                _immediateContext = NULL;
	ID3D11RenderTargetView*             _renderTargetView = NULL;
	ID3D11Texture2D*                    _depthStencil = NULL;
	ID3D11DepthStencilView*             _depthStencilView = NULL;
	ID3D11Texture2D*                    _shadowMapDepthStencil = NULL;
	ID3D11DepthStencilView*             _shadowMapDepthStencilView = NULL;
	bool								_isWireFrame;

	static const float s_clearColor[4]; //use this color to clear screen
	virtual bool init(int width, int height, HWND hWnd);
	virtual GfxInputLayoutPtr initInputLayout(const VertexInputlayoutDescription& description) override;
	bool initDevice(int width, int height, HWND hWnd);
	bool initDepthStencil(int width, int height);
	bool initState();
	virtual void clearDevice() override;
};

