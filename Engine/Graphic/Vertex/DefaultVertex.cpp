#include "Graphic/precomp.h"
#include "DefaultVertex.h"
#include "InputLayoutDefine.h"


std::vector<InputLayoutStruct> DefaultVertex::getInputLayoutStruct()
{
	return std::vector<InputLayoutStruct>{
		{"POSITION", InputLayoutEleType::VECTOR3}
	};
}
