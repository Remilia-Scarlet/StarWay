#pragma once
enum class IndexBufferDataFormat
{
	BIT_16,
	BIT_32
};
class GfxIndexBuffer
{
public:
	GfxIndexBuffer();
	virtual ~GfxIndexBuffer();

	bool initBuffer(const void* indexBuffer, int size,IndexBufferDataFormat dataFormat);
	void setBuffer();
protected:
	int _size;
	IndexBufferDataFormat _dataFormat;
	PlatformIndexBuffer* _indexBuffer;
};

