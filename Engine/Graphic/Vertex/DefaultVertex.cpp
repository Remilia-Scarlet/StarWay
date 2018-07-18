#include "Graphic/precomp.h"
#include "DefaultVertex.h"
#include "InputLayoutDefine.h"


const VertexInputlayoutDescription& CommonVertex::getInputLayoutDescription()
{
	static VertexInputlayoutDescription description{
		{ "POSITION", InputLayoutEleType::VECTOR3 },
		{ "NORMAL", InputLayoutEleType::VECTOR3 },
		{ "TEXCOORD", InputLayoutEleType::VECTOR2 }
	};
	return description;
}
