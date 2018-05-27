#pragma once
#include <string>
#include "../../../Engine/Ash/FileSystem/Path_Win.h"
#include <vector>
#include "../../../Engine/Ash/FileSystem/File_Win.h"
#include <map>
#include "../../../Engine/TinyEngine/ThirdParty/rapidjson/document.h"
#include "Ash/CommandLineCfg/CommandLineCfg.h"

class MyCommandLineCfg : public CommandLineCfg
{
public:
	DEFINE_COMMANDLINE_STR(Output, "", "/Output:OutputFile. Give the shader output file path and name");
	DEFINE_COMMANDLINE_STR(Source, "", "/Source:The hlsl file. Give the shader output file path and name");
};

class ShaderCompiler
{
public:
	ShaderCompiler();
	~ShaderCompiler();

	int run(const std::string& cmdLine);
protected:
	bool parseArg(const std::string& cmdLine);
	MyCommandLineCfg _cmdLine;
};

