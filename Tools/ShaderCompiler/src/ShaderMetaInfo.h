#pragma once
#include <string>  
#include <vector>
#include "Ash/FileSystem/fs_include.h"
#include "Ash/TinyAssert.h"
#include <boost/serialization/vector.hpp>

enum class ShaderType
{
	VS,
	PS,

	COUNT
};

struct ShaderDeclear
{
	std::string _name;
	ShaderType _shaderType = ShaderType::VS;
	std::string _entry;
	std::vector<std::string> _defines;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _name;
		ar & _shaderType;
		ar & _entry;
		ar & _defines;
	}
	
	Path calculateOutputFileName(const Path& outputDirectory) const 
	{
		return outputDirectory.getRelativePath() 
		+ (outputDirectory.getRelativePath().back() != '\\' ? "\\" : "")
		+ _name
		+ (_shaderType == ShaderType::VS ? ".vs" : ".ps") + ".cso";
	}
};

struct ShaderMetaInfo
{
	std::vector<ShaderDeclear> _declears;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _declears;
	}
};
