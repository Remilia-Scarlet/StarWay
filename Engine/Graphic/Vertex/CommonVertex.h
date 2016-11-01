#pragma once
#include "Graphic/GfxDefines.h"
#include "Math/vector/Vector.h"
#include <vector>

struct InputLayoutStruct;
class CommonVertex
{
public:
	CommonVertex(const Vector3& pos, const Vector2& uv)
		:pos(pos)
		,uv(uv)
	{}
	Vector3 pos;
	Vector2 uv;
	static std::vector<InputLayoutStruct> getInputLayoutStruct();
};

