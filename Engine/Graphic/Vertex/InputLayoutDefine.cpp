#include "Graphic/precomp.h"
#include "InputLayoutDefine.h"
#include <vector>
#include "DefaultVertex.h"

VertexInputlayoutDescription::VertexInputlayoutDescription(std::initializer_list<InputLayoutStruct> description)
	: _description(std::move(description))
{
	
}
