#pragma once
#include <vector>
#include "Graphic/GfxDefines.h"
#include "GfxShaderVertex.h"
#include "Ash/RefCountPointer/RefCountObj.h"
#include "GfxIndexBuffer.h"
#include "GfxVertexBuffer.h"
#include "Math/vector/Vector.h"
#include "GfxMaterial.h"
#include "GfxInputLayout.h"

TINY_DEFINE_PTR(GfxMesh);
class GfxMesh : public RefCountObj
{
protected:
	GfxMesh();
	virtual ~GfxMesh();

public:
	static GfxMeshPtr create(const GfxInputLayoutPtr& inputLayoutType, const void* vertexBuffer, int vertexBufferSize, int vertexBufferStride, const void* indexBuffer, int indexBufferSize, IndexBufferDataFormat dataFormat);

public:
	const GfxInputLayoutPtr& getInputLayoutType() const { return _inputLayoutType; }
	void setInputLayoutType(const GfxInputLayoutPtr& val) { _inputLayoutType = val; }

	const GfxVertexBuffer& getVertexBuffer() const { return _vertexBuffer; }
	void setVertexBuffer(const GfxVertexBuffer& val) { _vertexBuffer = val; }

	const GfxIndexBuffer& getIndexBuffer() const { return _indexBuffer; }
	void setIndexBuffer(const GfxIndexBuffer& val) { _indexBuffer = val; }

	int getVertexNumber() const { return _vertexNumber; }
	void setVertexNumber(int val) { _vertexNumber = val; }

	void setPrimitiveTopology(PrimitiveTopology primitiveTopology);
	PrimitiveTopology getPrimitiveTopology() const;

	void render(const GfxShaderVertexPtr& vs);
private:
	virtual bool init(const GfxInputLayoutPtr& inputLayoutType, const void* vertexBuffer, int vertexBufferSize, int vertexBufferStride, const void* indexBuffer, int indexBufferSize, IndexBufferDataFormat dataFormat);
	GfxInputLayoutPtr _inputLayoutType;
	GfxVertexBuffer _vertexBuffer;
	GfxIndexBuffer _indexBuffer;
	int _vertexNumber;
};


