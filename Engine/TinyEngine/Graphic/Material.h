#pragma once
#include "GraphicCommand.h"

class DataStream;

class Material : public RefCountObj
{
public:
	bool serialize(DataStream* dataStream);
	bool unserialize(DataStream* dataStream);

protected:
	std::string _shaderName;
	std::vector<GraphicCommand> _command; //We input some info here, for example textures and constants
};
