#pragma once
#include <string>  
#include <vector>
#include "Ash/FileSystem/fs_include.h"
#include "Ash/TinyAssert.h"
class Path;

enum class ShaderType
{
	VS,
	PS
};

struct ShaderDeclear
{
	std::string _name;
	ShaderType _shaderType = ShaderType::VS;
	std::string _entry;
	std::vector<std::string> _defines;
	
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
};
