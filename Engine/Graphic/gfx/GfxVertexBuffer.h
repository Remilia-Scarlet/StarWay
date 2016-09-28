#pragma once
class GfxVertexBuffer
{
public:
	GfxVertexBuffer();
	virtual ~GfxVertexBuffer();
	bool initBuffer(const void* vertexBufferData, int size, int stride);
	void setBuffer();
protected:
	int _vertexBufferSize;
	int _vertexBufferStride;
	PlatformVertexBuffer* _vertexBuffer;
};

