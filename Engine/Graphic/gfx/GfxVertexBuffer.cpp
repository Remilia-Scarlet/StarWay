#include "Graphic\precomp.h"
#include "GfxVertexBuffer.h"
#include "Graphic\Manager\GraphicMgr.h"
#include "Graphic\DX11\DX11GraphicMgr.h"


GfxVertexBuffer::GfxVertexBuffer()
	: _vertexBufferSize(0)
	, _vertexBufferStride(0)
	, _vertexBuffer(nullptr)
{
}


GfxVertexBuffer::~GfxVertexBuffer()
{
	TINY_SAFE_RELEASE(_vertexBuffer);
}

bool GfxVertexBuffer::initBuffer(const void* vertexBufferData, int size, int stride)
{
#if TINY_GRAPHIC_ENGINE_TARGET == TINY_GRAPHIC_ENGINE_DX11
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = size;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = vertexBufferData;
	ID3D11Device* device = GraphicMgr::instance()->getD3D11Device();
	if (!device)
	{
		DebugString("create vertex buffer failed");
		return false;
	}
	HRESULT hr = device->CreateBuffer(&bd, &InitData, &_vertexBuffer);
	if (FAILED(hr))
	{
		DebugString("create vertex buffer failed!");
		return false;
	}
	SET_DEBUG_NAME(_vertexBuffer,"VertexBuffer");
	return true;
#else
#error Not supported
#endif
}

void GfxVertexBuffer::setBuffer()
{
	UINT ustride = UINT(_vertexBufferStride);
	UINT offset = 0;
	ID3D11DeviceContext* context = GraphicMgr::instance()->getD3D11DeviceContext();
	TinyAssert(context != nullptr, "GfxVertexBuffer::setBuffer can't get context");
	context->IASetVertexBuffers(0, 1, &_vertexBuffer, &ustride, &offset);
}
