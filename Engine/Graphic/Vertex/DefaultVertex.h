#pragma once
#include "Graphic/GfxDefines.h"
#include "Math/vector/Vector.h"
#include <vector>
#include "InputLayoutDefine.h"

class CommonVertex
{
	TINY_VERTEX(CommonVertex)
public:
	CommonVertex() {}
	CommonVertex(const Vector3& pos, Vector3 normal, const Vector2& uv)
		:pos(pos)
		, normal(normal)
		, uv(uv)
	{}
	Vector3 pos;
	Vector3 normal;
	Vector2 uv;

	static const VertexInputlayoutDescription& getInputLayoutDescription();
};

