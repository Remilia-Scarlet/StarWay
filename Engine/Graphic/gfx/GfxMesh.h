#pragma once
#include <vector>
#include "Graphic/GfxDefines.h"
#include "GfxShaderVertex.h"
#include "Ash/RefCountPointer/RefCountObj.h"
#include "GfxIndexBuffer.h"
#include "GfxVertexBuffer.h"
#include "Math/vector/Vector.h"
enum class InputLayoutType;

struct Material
{
	Vector4 ambient;
	Vector4 diffuse;
	Vector4 specular;
	Vector4 reflect;
};

TINY_DEFINE_PTR(GfxMesh);
class GfxMesh : public RefCountObj
{
protected:
	GfxMesh();
	virtual ~GfxMesh();

public:
	static GfxMeshPtr create(InputLayoutType inputLayoutType, const void* vertexBuffer, int vertexBufferSize, int vertexBufferStride, const void* indexBuffer, int indexBufferSize, IndexBufferDataFormat dataFormat);

public:
	InputLayoutType getInputLayoutType() const { return _inputLayoutType; }
	void setInputLayoutType(InputLayoutType val) { _inputLayoutType = val; }

	const GfxVertexBuffer& getVertexBuffer() const { return _vertexBuffer; }
	void setVertexBuffer(const GfxVertexBuffer& val) { _vertexBuffer = val; }

	const GfxIndexBuffer& getIndexBuffer() const { return _indexBuffer; }
	void setIndexBuffer(const GfxIndexBuffer& val) { _indexBuffer = val; }

	int getVertexNumber() const { return _vertexNumber; }
	void setVertexNumber(int val) { _vertexNumber = val; }

	void setMaterial(const Material& material);
	const Material& getMaterial();

	void render(const GfxShaderVertexPtr& vs);
private:
	virtual bool init(InputLayoutType inputLayoutType, const void* vertexBuffer, int vertexBufferSize, int vertexBufferStride, const void* indexBuffer, int indexBufferSize, IndexBufferDataFormat dataFormat);
	InputLayoutType _inputLayoutType;
	GfxVertexBuffer _vertexBuffer;
	GfxIndexBuffer _indexBuffer;
	int _vertexNumber;
	Material _material;
};


