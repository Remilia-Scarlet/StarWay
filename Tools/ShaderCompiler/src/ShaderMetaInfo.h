#pragma once
#include <string>  
#include <vector>
#include "FileSystem/Path_Win.h"
#include "TinyAssert.h"
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
		return outputDirectory.getRelativePath() + "\\" + _name + (_shaderType == ShaderType::VS ? ".vs" : ".ps") + ".cso";
	}
};

struct ShaderMetaInfo
{
	struct ParamInfo
	{
		int _index = 0;
		std::string _name;
	};
	std::vector<ParamInfo> _params;
	std::vector<ShaderDeclear> _declears;
};
