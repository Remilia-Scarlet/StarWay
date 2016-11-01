#pragma once
#include <string>
#include <vector>

class CommonVertex;
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

std::vector<std::vector<InputLayoutStruct>>  GetInputLayoutDescs();
