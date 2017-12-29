#pragma once
#include <string>
#include "..\..\..\Engine\Ash\FileSystem\Path_Win.h"
#include <vector>
#include "..\..\..\Engine\Ash\FileSystem\File_Win.h"
#include <map>
#include "..\..\..\Engine\TinyEngine\ThirdParty\rapidjson\document.h"

class CommonCompiler
{
public:
	CommonCompiler();
	~CommonCompiler();

	int run(int argc, char* argv[]);
protected:
	bool parseArg(int argc, char* argv[]);
	bool readConfigFile();
	bool changeDefine();
	bool checkPath();
	bool compile();



	bool compileFile(const WIN32_FIND_DATA& info,const Path& path, const std::string& subFolder);
	bool flushLog();
	std::string fixRuntimeDefine(const WIN32_FIND_DATA& info,const std::string& def, const std::string& subFolder);
	bool callCmdAndWaitFinish(const WIN32_FIND_DATA& info, const std::string& cmd, const std::string& subFolder);
	bool checkTimeStamp(const Path& srcPath, const WIN32_FIND_DATA& srcInfo, const Path& desPath);
	bool writeTimeStamp(const Path& srcPath, const WIN32_FIND_DATA& srcInfo, const Path& desPath);

	bool _forceRecompile;
	std::string _configFilePath;
	rapidjson::Document _logDoc;
	Path _sourcePath;
	Path _cmdExe;
	File _logFile;

	std::map<std::string, std::string> _define;
	std::map<std::string, std::string> _runtimeDefine;
};

