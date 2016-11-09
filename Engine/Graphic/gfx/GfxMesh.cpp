#include "Graphic\precomp.h"
#include "GfxMesh.h"
#include "Graphic\Manager\ConstantBufferManager.h"
#include "Graphic\DX11\DX11GraphicMgr.h"


GfxMesh::GfxMesh()
	:_vertexNumber(0)
{
}


GfxMesh::~GfxMesh()
{
}

void GfxMesh::setMaterial(const Material& material)
{
	_material = material;
}

const Material& GfxMesh::getMaterial()
{
	return _material;
}

void GfxMesh::setPrimitiveTopology(PrimitiveTopology primitiveTopology)
{
	_indexBuffer.setPrimitiveTopology(primitiveTopology);
}

PrimitiveTopology GfxMesh::getPrimitiveTopology() const
{
	return _indexBuffer.getPrimitiveTopology();
}

void GfxMesh::render(const GfxShaderVertexPtr& vs)
{
	ConstantBufferManager::instance()->setVSVector(13, _material.ambient);
	ConstantBufferManager::instance()->setVSVector(14, _material.diffuse);
	ConstantBufferManager::instance()->setVSVector(15, _material.specular);
	ConstantBufferManager::instance()->setVSVector(16, _material.reflect);

	GraphicMgr::instance()->setInputLayout(_inputLayoutType);
	_vertexBuffer.setBuffer();
	_indexBuffer.setBuffer();
	GraphicMgr::instance()->setDrawIndexNumber(_vertexNumber);
}

GfxMeshPtr GfxMesh::create(InputLayoutType inputLayoutType, const void* vertexBuffer, int vertexBufferSize, int vertexBufferStride, const void* indexBuffer, int indexBufferSize, IndexBufferDataFormat dataFormat)
{
	GfxMesh* ret = new GfxMesh();
	if (!ret || !ret->init(inputLayoutType, vertexBuffer, vertexBufferSize, vertexBufferStride, indexBuffer, indexBufferSize, dataFormat))
		TINY_SAFE_DELETE(ret);
	return GfxMeshPtr(ret);
}

bool GfxMesh::init(InputLayoutType inputLayoutType, const void* vertexBuffer, int vertexBufferSize, int vertexBufferStride, const void* indexBuffer, int indexBufferSize, IndexBufferDataFormat dataFormat)
{
	do
	{
		_inputLayoutType = inputLayoutType;
		_vertexBuffer.initBuffer(vertexBuffer, vertexBufferSize, vertexBufferStride);
		_indexBuffer.initBuffer(indexBuffer, indexBufferSize, dataFormat);
		_vertexNumber = indexBufferSize / (dataFormat == IndexBufferDataFormat::BIT_16 ? 2 : 4);
		return true;
	} while (0);
	return false;
}
