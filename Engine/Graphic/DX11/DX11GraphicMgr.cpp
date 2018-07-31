#include "Graphic/precomp.h"
#include "DX11GraphicMgr.h"
#include "Graphic/gfx/GfxShaderVertex.h"
#include "Graphic/gfx/GfxShaderPixel.h"
#include "Graphic/Manager/ConstantBufferManager.h"
#include "Graphic/Manager/ShaderMgr.h"
#include <functional>
#include "Graphic/Vertex/InputLayoutDefine.h"
#include "Graphic/Manager/LightManager.h"
#include "DX11InputLayout.h"
#include "DX11RenderBuffer.h"

const float DX11GraphicMgr::s_clearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
void DX11GraphicMgr::setViewPort(const ViewPort& viewPort)
{
	D3D11_VIEWPORT vp;
	vp.Width = viewPort.Width;
	vp.Height = viewPort.Height;
	vp.MinDepth = viewPort.MinDepth;
	vp.MaxDepth = viewPort.MaxDepth;
	vp.TopLeftX = viewPort.TopLeftX;
	vp.TopLeftY = viewPort.TopLeftY;
	_immediateContext->RSSetViewports(1, &vp);
}

ViewPort DX11GraphicMgr::getViewPort()
{
	UINT num;
	D3D11_VIEWPORT vp;
	_immediateContext->RSGetViewports(&num, &vp);
	return ViewPort(vp.TopLeftX, vp.TopLeftY, vp.Width, vp.Height, vp.MinDepth, vp.MaxDepth);
}

void DX11GraphicMgr::preRender()
{
	// Clear the back buffer
	_immediateContext->ClearRenderTargetView(_renderTargetView, s_clearColor);

	// Clear the depth buffer to 1.0 (max depth)
	_immediateContext->ClearDepthStencilView(_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// reset draw index number
	_drawIndexNumber = 0;

	LightManager::instance()->preRender();
}

void DX11GraphicMgr::draw()
{
	// TODO : optimize state management
	ID3D11RasterizerState *	rasterState = NULL;
	/*	_immediateContext->RSGetState(&rasterState);*/
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	/*	rasterState->GetDesc(&desc);*/
	desc.CullMode = D3D11_CULL_NONE;
	desc.FillMode = _isWireFrame ? D3D11_FILL_WIREFRAME : D3D11_FILL_SOLID;
	_d3dDevice->CreateRasterizerState(&desc, &rasterState);
	_immediateContext->RSSetState(rasterState);
	rasterState->Release();

	if(_drawIndexNumber > 0)
		_immediateContext->DrawIndexed(_drawIndexNumber, 0, 0);
}

void DX11GraphicMgr::render()
{
	

	// Present our back buffer to our front buffer
	_swapChain->Present(0, 0);
}

void DX11GraphicMgr::setPixelShader(const GfxShaderPixelPtr& ps)
{
	if (ps.isValid())
		_immediateContext->PSSetShader(ps->getPlatformPSShader(), NULL, 0);
	else
		TinyAssert(false, "DX11GraphicMgr::setPixelShader no shader");
}

void DX11GraphicMgr::setVertexShader(const GfxShaderVertexPtr& vs)
{
	if(vs.isValid())
		_immediateContext->VSSetShader(vs->getPlatformVSShader(), NULL, 0);
	else
		TinyAssert(false, "DX11GraphicMgr::setVertexShader no shader");
}

void DX11GraphicMgr::setWireFrame(bool isWireFrame)
{
	_isWireFrame = isWireFrame;
}

void DX11GraphicMgr::setRenderBuffer(const GfxRenderBufferPtr& buffer)
{
	if (!buffer.isValid())
		return;

	DX11RenderBufferPtr dx11buffer = StaticRefCountCast<DX11RenderBuffer>(buffer);
	ID3D11RenderTargetView* renderTargetView = dx11buffer->getRenderTargetView();
	_immediateContext->OMSetRenderTargets(1, &renderTargetView, _depthStencilView);

	setViewPort({ 0.f,0.f,(float)SHADOW_MAP_WIDTH,(float)SHADOW_MAP_HEIGHT,0.f,1.f });
}

void DX11GraphicMgr::resetRenderBuffer()
{
	_immediateContext->OMSetRenderTargets(1, &_renderTargetView, _depthStencilView);
}

GfxInputLayoutPtr DX11GraphicMgr::initInputLayout(const VertexInputlayoutDescription& description)
{
	bool ret = true;
	struct InputElementInfo
	{
		InputElementInfo(DXGI_FORMAT format, const char* typeInfo, int size)
			:_format(format), _typeInfo(typeInfo),_size(size){}
		DXGI_FORMAT _format;
		const char* _typeInfo;
		int _size;
	};
	std::function<InputElementInfo(InputLayoutEleType)> convertInputLayoutEleTypeFun = [](InputLayoutEleType inputLayoutType) {
		switch (inputLayoutType)
		{
		case InputLayoutEleType::FLOAT:
			return InputElementInfo(DXGI_FORMAT_R32_FLOAT,"float",4 );
		case InputLayoutEleType::VECTOR2:
			return InputElementInfo(DXGI_FORMAT_R32G32_FLOAT, "float2", 8);
		case InputLayoutEleType::VECTOR3:
			return InputElementInfo(DXGI_FORMAT_R32G32B32_FLOAT, "float3", 12);
		case InputLayoutEleType::VECTOR4:
			return InputElementInfo(DXGI_FORMAT_R32G32B32A32_FLOAT, "float4", 16);
		default:
			return InputElementInfo(DXGI_FORMAT_R32G32B32A32_FLOAT, "float4", 16);
		}
	};

	std::vector<D3D11_INPUT_ELEMENT_DESC> layout;
	std::string shaderCode =
		"struct VS_INPUT\n"
		"{\n"
		;
	int valueNumber = 0;
	const std::vector<InputLayoutStruct>& desc = description.getDescription();
	for (const InputLayoutStruct& desc : desc)
	{
		layout.push_back({desc._semanticName, 0, convertInputLayoutEleTypeFun(desc._type)._format, 0, (UINT)valueNumber, D3D11_INPUT_PER_VERTEX_DATA, 0});
		shaderCode += convertInputLayoutEleTypeFun(desc._type)._typeInfo;
		shaderCode += FormatString(" value%d : %s;\n", valueNumber, desc._semanticName);
		valueNumber += convertInputLayoutEleTypeFun(desc._type)._size;
	}
	shaderCode +=
		"};\n"
		"float4 main(VS_INPUT input) : SV_POSITION\n"
		"{\n"
		"return float4(0,0,0,0);\n"
		"}\n"
		;
	ID3DBlob* shaderBlob = nullptr;
	bool successed = false; //DX11Wrapper::CompileShader(shaderCode.c_str(), (int)shaderCode.length(), "main","vs_5_0", &shaderBlob);
	TinyAssert(successed);

	ID3D11InputLayout* d3dInputLayout = nullptr;
	if (successed)
	{
		// Create the actual input layout
		HRESULT hr = _d3dDevice->CreateInputLayout(layout.data(),
			(UINT)layout.size(),
			shaderBlob->GetBufferPointer(),
			(UINT)shaderBlob->GetBufferSize(),
			&d3dInputLayout);
		TINY_SAFE_RELEASE(shaderBlob);
		if (FAILED(hr))
			ret = false;
		else
			SET_DEBUG_NAME(d3dInputLayout, "Inputlayout");
	}
	else
		ret = false;
	if (ret)
	{
		PlatformInputLayoutPtr layout = DX11InputLayout::create(d3dInputLayout);
		return layout;
	}
	else
		return nullptr;
}


bool DX11GraphicMgr::initDevice(int width, int height, HWND hWnd)
{
	HRESULT hr = S_OK;
	UINT createDeviceFlags = 0;
#ifndef TINY_RETAIL
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	UINT numDriverTypes = ARRAYSIZE(driverTypes);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	UINT numFeatureLevels = ARRAYSIZE(featureLevels);

	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
	{
		_driverType = driverTypes[driverTypeIndex];
		hr = D3D11CreateDeviceAndSwapChain(NULL, _driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &sd, &_swapChain, &_d3dDevice, &_featureLevel, &_immediateContext);
		if (SUCCEEDED(hr))
			break;
	}
	if (FAILED(hr))
		return false;
	SET_DEBUG_NAME(_swapChain,"SwapChain");
	SET_DEBUG_NAME(_d3dDevice,"Device");
	SET_DEBUG_NAME(_immediateContext, "ImmediateContext");
	// Create a render target view
	ID3D11Texture2D* pBackBuffer = NULL;
	hr = _swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr))
		return false;

	hr = _d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &_renderTargetView);
	SET_DEBUG_NAME(_renderTargetView, "RenderTargetView");
	TINY_SAFE_RELEASE(pBackBuffer);
	if (FAILED(hr))
		return false;
	return true;
}

bool DX11GraphicMgr::initDepthStencil(int width, int height)
{
	HRESULT hr = S_OK;
	// Create depth stencil texture
	D3D11_TEXTURE2D_DESC descDepth;
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = width;
	descDepth.Height = height;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	hr = _d3dDevice->CreateTexture2D(&descDepth, NULL, &_depthStencil);
	if (FAILED(hr))
		return false;
	SET_DEBUG_NAME(_depthStencil, "DepthStencil");

	// Create the depth stencil view
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	hr = _d3dDevice->CreateDepthStencilView(_depthStencil, &descDSV, &_depthStencilView);
	if (FAILED(hr))
		return false;
	SET_DEBUG_NAME(_depthStencilView,"DepthStencilView");
	return true;
}

bool DX11GraphicMgr::initState()
{
	do 
	{
		HRESULT hr = S_OK;
		return true;
	} while (0);
	return false;
}

DX11GraphicMgr::DX11GraphicMgr()
	:_isWireFrame(false)
{
}


DX11GraphicMgr::~DX11GraphicMgr()
{
}

bool DX11GraphicMgr::init(int width, int height, HWND hWnd)
{
	do 
	{
		//init device, swapchain and rendertargetview
		TINY_BREAK_IF(!initDevice(width, height, hWnd));
		//init DepthStencil
		TINY_BREAK_IF(!initDepthStencil(width, height));

		return true;
	} while (0);
	return false;
}

void DX11GraphicMgr::clearDevice()
{
	if (_immediateContext) _immediateContext->ClearState();

	TINY_SAFE_RELEASE(_depthStencil);
	TINY_SAFE_RELEASE(_depthStencilView);
	TINY_SAFE_RELEASE(_renderTargetView);
	TINY_SAFE_RELEASE(_swapChain);
	TINY_SAFE_RELEASE(_immediateContext);
	TINY_SAFE_RELEASE(_d3dDevice);
}
