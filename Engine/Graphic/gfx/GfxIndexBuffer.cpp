#include "precomp.h"
#include "GfxIndexBuffer.h"
#include "Graphic/DX11/DX11GraphicMgr.h"

GfxIndexBuffer::GfxIndexBuffer()
	: _size(0)
	, _indexBuffer(nullptr)
	,_dataFormat(IndexBufferDataFormat::BIT_16)
	,_primitiveTopology(PrimitiveTopology::TRIANGLE_LIST)
{
}


GfxIndexBuffer::~GfxIndexBuffer()
{
	TINY_SAFE_RELEASE(_indexBuffer);
}

bool GfxIndexBuffer::initBuffer(const void* indexBuffer, int size, IndexBufferDataFormat dataFormat)
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = UINT(size);
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indexBuffer;
	ID3D11Device* device = GraphicMgr::instance()->getD3D11Device();
	if (!device)
	{
		DebugString("create index buffer failed");
		return false;
	}
	HRESULT hr = device->CreateBuffer(&bd, &InitData, &_indexBuffer);
	if (FAILED(hr))
	{
		DebugString("create index buffer failed");
		return false;
	}
	SET_DEBUG_NAME(_indexBuffer, FormatString("IndexBuffer_size:%d",size).c_str());
	_dataFormat = dataFormat;
	return true;
}

void GfxIndexBuffer::setBuffer()
{
#if defined(TINY_PLATFORM_WINDOWS)
	ID3D11DeviceContext* context = GraphicMgr::instance()->getD3D11DeviceContext();
	TinyAssert(context != nullptr, "GfxIndexBuffer::setBuffer can't get context");
	context->IASetIndexBuffer(_indexBuffer, (_dataFormat == IndexBufferDataFormat::BIT_16 ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT), 0);

	// Set primitive topology
	struct ToDxTopology
	{
		static D3D_PRIMITIVE_TOPOLOGY run(PrimitiveTopology topo)
		{
			switch (topo)
			{
			case PrimitiveTopology::POINT_LIST:
				return D3D11_PRIMITIVE_TOPOLOGY_POINTLIST;
			case PrimitiveTopology::LINE_LIST:
				return D3D11_PRIMITIVE_TOPOLOGY_LINELIST;
			case PrimitiveTopology::LINE_STRIP:
				return D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP;
			case PrimitiveTopology::TRIANGLE_LIST:
				return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
			case PrimitiveTopology::TRIANGLE_STRIP:
				return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
			}
			return D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
		}
	};
	context->IASetPrimitiveTopology(ToDxTopology::run(_primitiveTopology));
#else
#error not support
#endif
}

void GfxIndexBuffer::setPrimitiveTopology(PrimitiveTopology primitiveTopology)
{
	_primitiveTopology = primitiveTopology;
}

PrimitiveTopology GfxIndexBuffer::getPrimitiveTopology() const
{
	return _primitiveTopology;
}
