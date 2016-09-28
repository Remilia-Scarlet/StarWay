#pragma once
#include "Graphic/GfxDefines.h"
#include "Math/vector/Vector.h"
class StarVertex
{
public:
	StarVertex(const Vector3& pos, const Vector2& uv)
		:pos(pos)
		,uv(uv)
	{}
	Vector3 pos;
	Vector2 uv;
public:
	StarVertex();
	~StarVertex();
};

