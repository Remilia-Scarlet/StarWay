#pragma once
#include <string>
#include <vector>

#define TINY_VERTEX(CLASS_NAME) public:static const char* getName(){return #CLASS_NAME;}

enum class InputLayoutEleType
{
	FLOAT,
	VECTOR2,
	VECTOR3,
	VECTOR4
};
struct InputLayoutStruct
{
	const char* _semanticName;
	InputLayoutEleType _type;
};

class VertexInputlayoutDescription
{
public:
	VertexInputlayoutDescription(std::initializer_list<InputLayoutStruct> description);

	const std::vector<InputLayoutStruct>& getDescription() const { return _description; }
protected:
	std::vector<InputLayoutStruct> _description;
};