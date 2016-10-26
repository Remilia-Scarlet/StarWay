#pragma once
#include <string>
#include "..\..\..\Engine\Ash\FileSystem\Path_Win.h"
#include <vector>
#include "..\..\..\Engine\Ash\FileSystem\File_Win.h"
#include <map>

class CommonCompiler
{
public:
	CommonCompiler();
	~CommonCompiler();

	bool parseArg(int argc, char* argv[]);
	bool readConfigFile();
	bool compile();

	const Path& getConfigFilePath() const;
protected:
	std::string _configFilePath;
	std::string _sourcePath;
	std::string _sourceFilter;
	std::string _tempFile;
	std::vector<std::string> _dependency;
	std::string _output;
	std::string _cmd;

	std::map<std::string, std::string> _define;
};

