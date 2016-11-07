#pragma once
#include "Graphic/GfxDefines.h"
#include "Math/vector/Vector.h"
#include <vector>

struct InputLayoutStruct;
class CommonVertex
{
public:
	CommonVertex() {}
	CommonVertex(const Vector3& pos, Vector3 normal, const Vector2& uv)
		:pos(pos)
		,normal(normal)
		,uv(uv)
	{}
	Vector3 pos;
	Vector3 normal;
	Vector2 uv;
	static std::vector<InputLayoutStruct> getInputLayoutStruct();
};

