#pragma once
#include <string>
#include "..\..\..\Engine\Ash\FileSystem\Path_Win.h"
#include <vector>

class CommonCompiler
{
public:
	CommonCompiler();
	~CommonCompiler();

	bool parseArg(int argc, char* argv[]);
	bool readConfigFile();


	const Path& getConfigFilePath() const;
protected:
	Path _configFilePath;
	Path _sourcePath;
	std::string _sourceFilter;
	Path _tempFile;
	std::vector<Path> _dependency;
	Path _output;
	std::string _cmd;
};

