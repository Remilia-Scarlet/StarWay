#pragma once
#include <string>
#include "..\..\..\Engine\Ash\FileSystem\Path_Win.h"
#include <vector>
#include "..\..\..\Engine\Ash\FileSystem\File_Win.h"
#include <map>
#include "..\..\..\Engine\ThirdParty\rapidjson\document.h"

class CommonCompiler
{
public:
	CommonCompiler();
	~CommonCompiler();

	int run(int argc, char* argv[]);

	const std::string& getConfigFilePath() const;
	std::string getLogFilePath() const;
	const std::string& getSourcePath() const;
protected:
	bool parseArg(int argc, char* argv[]);
	bool readConfigFile();
	bool changeDefine();
	bool compile();
	bool openLogFile();
	bool buildSourcePath();

	bool compileFile(const WIN32_FIND_DATA& info,const Path& path);
	bool flushLog();
	std::string fixRuntimeDefine(const WIN32_FIND_DATA& info,const std::string& def);

	Path _configFilePath;
	std::vector<std::string> _dependency;
	rapidjson::Document _logDoc;
	Path _sourcePath;
	File _logFile;

	std::map<std::string, std::string> _define;
	std::map<std::string, std::string> _runtimeDefine;
};

