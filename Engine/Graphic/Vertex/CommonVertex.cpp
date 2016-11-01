#include "Graphic/precomp.h"
#include "CommonVertex.h"
#include "InputLayoutDefine.h"


std::vector<InputLayoutStruct> CommonVertex::getInputLayoutStruct()
{
	return std::vector<InputLayoutStruct>{
		{"POSITION", InputLayoutEleType::VECTOR3}, 
		{"TEXCOORD", InputLayoutEleType::VECTOR2}
	};
}
