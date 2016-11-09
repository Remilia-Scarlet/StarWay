#pragma once
enum class IndexBufferDataFormat
{
	BIT_16,
	BIT_32
};

enum class PrimitiveTopology
{
	POINT_LIST,
	LINE_LIST,
	LINE_STRIP,
	TRIANGLE_LIST,
	TRIANGLE_STRIP
};

class GfxIndexBuffer
{
public:
	GfxIndexBuffer();
	virtual ~GfxIndexBuffer();

	bool initBuffer(const void* indexBuffer, int size,IndexBufferDataFormat dataFormat);
	void setBuffer();

	void setPrimitiveTopology(PrimitiveTopology primitiveTopology);
	PrimitiveTopology getPrimitiveTopology() const;
protected:
	int _size;
	IndexBufferDataFormat _dataFormat;
	PlatformIndexBuffer* _indexBuffer;
	PrimitiveTopology _primitiveTopology;
};

