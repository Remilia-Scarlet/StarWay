#pragma once
#include "Graphic/GfxDefines.h"
#include "Math/vector/Vector.h"
#include <vector>

struct InputLayoutStruct;
class DefaultVertex
{
public:
	DefaultVertex() {}
	DefaultVertex(const Vector3& pos)
		:pos(pos)
	{}
	Vector3 pos;
	static std::vector<InputLayoutStruct> getInputLayoutStruct();
};

