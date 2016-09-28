#pragma once
#include <string>
#include <vector>

enum class InputLayoutEleType
{
	FLOAT,
	VECTOR2,
	VECTOR3,
	VECTOR4
};
struct InputLayoutStruct
{
	std::string _semanticName;
	InputLayoutEleType _type;
};

enum class InputLayoutType
{
	DEFAULT,
	COMMON,
	TYPE_NUMBER
};

static const std::vector<InputLayoutStruct> INPUT_LAYOUT_DESCS[static_cast<size_t>(InputLayoutType::TYPE_NUMBER)] = 
{
	//InputLayoutType::DEFAULT
	{
		{ "POSITION",InputLayoutEleType::VECTOR4 },
	},
	//InputLayoutType::COMMON
	{
		{"POSITION",InputLayoutEleType::VECTOR3 },
		{"TEXCOORD",InputLayoutEleType::VECTOR2 }
	}
};