#pragma once
#include <string>
#include <vector>
#include "Ash/FileSystem/fs_include.h"

struct ParamInfo
{
	ParamInfo() = default;
	ParamInfo(const std::string& name, int slot, int size)
		:_name(name),_slot(slot),_size(size)
	{}
	std::string _name;
	int _slot = -1;
	int _size = -1;

	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _name;
		ar & _slot;
		ar & _size;
	}
};

class ShaderReflectionParser
{
public:
	ShaderReflectionParser(const Path& binaryShader);
	std::vector<ParamInfo> getParamInfo();
protected:
	Path _binaryShader;
};