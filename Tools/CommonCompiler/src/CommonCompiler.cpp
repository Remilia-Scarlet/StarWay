#define _CRT_SECURE_NO_WARNINGS
#include "CommonCompiler.h"
#include <fstream>
#include "Engine\ThirdParty\rapidjson\document.h"
#include "Engine\ThirdParty\rapidjson\writer.h"
#include "Engine\ThirdParty\rapidjson\fwd.h"
#include "Engine\ThirdParty\rapidjson\stringbuffer.h"
#include "Engine\Ash\FileSystem\File_Win.h"
#include "Engine\TinyEngine\Engine\EngineDefs.h"
#include "Engine\Ash\CommonFunc.h"
#include <list>
using namespace rapidjson;

const char* SOURCE_PATH_NAME = "$(SourcePath)";
const char* SOURCE_FILE_NAME = "$(SourceName)";
const char* SOURCE_EXT_NAME = "$(SourceExt)";
const char* SOURCE_FILTER_NAME = "$(SourceFilter)";
const char* LOG_FILE_NAME = "$(LogFile)";
const char* DEPENDENCY_NAME = "$(Dependency)";
const char* OUTPUT_NAME = "$(Output)";
const char* CMD_NAME = "$(Cmd)";

CommonCompiler::CommonCompiler()
{
}


CommonCompiler::~CommonCompiler()
{
}

int CommonCompiler::run(int argc, char* argv[])
{
	if (!parseArg(argc, argv))
		return 1;

	if (!readConfigFile())
		return 2;

	if (!changeDefine())
		return 3;

	if (!openLogFile())
		return 4;

	if (!buildSourcePath())
		return 5;

	if (!compile())
		return 6;
	
	return 0;
}

bool CommonCompiler::parseArg(int argc, char* argv[])
{
	for (int i = 1; i < argc; ++i)
	{
		if (strcmp(argv[i], "/?") == 0)
		{
			return false;
		}
		else if (strcmp(argv[i], "/config") == 0)
		{
			if (i + 1 < argc)
			{
				_configFilePath = argv[i + 1];
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	return false;
}

bool CommonCompiler::readConfigFile()
{
	File file;
	if (!file.open(_configFilePath, File::AccessMode::READ, File::CreateMode::OPEN_EXIST))
		return false;

	Document json;
	std::vector<char> data = file.readAll();
	json.Parse(data.data(),data.size());
	auto asd = json.GetParseError();
	if (json.GetParseError())
		return false;

	if (json.HasMember(SOURCE_PATH_NAME))
		_define[SOURCE_PATH_NAME] = json[SOURCE_PATH_NAME].GetString();

	if (json.HasMember(SOURCE_FILTER_NAME))
		_define[SOURCE_FILTER_NAME] = json[SOURCE_FILTER_NAME].GetString();

	if (json.HasMember(LOG_FILE_NAME))
		_define[LOG_FILE_NAME] = json[LOG_FILE_NAME].GetString();

	if (json.HasMember(DEPENDENCY_NAME))
	{
		Value& arr = json[DEPENDENCY_NAME];
		if (arr.IsString())
			_dependency.push_back(arr.GetString());
		else if (arr.IsArray())
		{
			for (Value& val : arr.GetArray())
				_dependency.push_back(val.GetString());
		}
	}

	if (json.HasMember(OUTPUT_NAME))
		_define[OUTPUT_NAME] = json[OUTPUT_NAME].GetString();

	if (json.HasMember(CMD_NAME))
		_define[CMD_NAME] = json[CMD_NAME].GetString();
	return true;
}


bool CommonCompiler::changeDefine()
{
	int loopRemainTimes = 50;
	while (--loopRemainTimes)
	{
		bool loop = false;
		for (auto it1 = _define.begin(); it1 != _define.end(); ++it1)
		{
			const std::string& key1 = it1->first;
			std::string& val1 = it1->second;
			for (auto it2 = _define.begin(); it2 != _define.end(); ++it2)
			{
				const std::string& key2 = it2->first;
				std::string& val2 = it2->second;
				if (val1.find(key2) != -1)
				{
					if (it2 == it1)
						return false;//loop define
					loop = true;
					replaceSubstr(val1, key2, val2);
				}
			}
		}
		if (!loop)
			break;
	}
	if (loopRemainTimes == 0)
		return false;
	return true;
}

bool CommonCompiler::compile()
{
	return folderWalker(_sourcePath.getAbsolutePath(), _define[SOURCE_FILTER_NAME]);
}

bool CommonCompiler::folderWalker(const std::string& path, const std::string& filter)
{
	WIN32_FIND_DATA att;
	HANDLE hFind = FindFirstFile((path + _define[SOURCE_FILTER_NAME]).c_str(), &att);
	if (hFind == INVALID_HANDLE_VALUE)
		return true;
	do
	{
		if (strcmp(att.cFileName, ".") == 0 || strcmp(att.cFileName, "..") == 0)
			continue;

		if (att.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			folderWalker(path + att.cFileName, filter);
		else
			compileFile(att, path);

	} while (::FindNextFile(hFind, &att));
	FindClose(hFind);
	return true;
}

bool CommonCompiler::compileFile(const WIN32_FIND_DATA& info, const std::string& path)
{
	return false;
}

bool CommonCompiler::flushLog()
{
	StringBuffer buffer;
	Writer<StringBuffer> writer(buffer);
	_logDoc.Accept(writer);
	const char* fileData = buffer.GetString();
	_logFile.open(_logFile.getFilePath());
	_logFile.seek(0);
	_logFile.write(fileData, (int)strlen(fileData));
	_logFile.close();
	return true;
}

bool CommonCompiler::openLogFile()
{
	if (!_logFile.open(_define[LOG_FILE_NAME],File::AccessMode::READ))
		return false;
	std::vector<char> data = _logFile.readAll();
	_logFile.close();
	_logDoc.Parse(data.data(), data.size());
	if (_logDoc.HasParseError())
		_logDoc.SetObject();
	flushLog();
	return true;
}

bool CommonCompiler::buildSourcePath()
{
	_sourcePath = _define[SOURCE_PATH_NAME];
	if (!_sourcePath.isDirectory())
		return false;
	return true;
}


const std::string& CommonCompiler::getConfigFilePath() const
{
	return _configFilePath.getAbsolutePath();
}

std::string CommonCompiler::getLogFilePath() const
{
	return Path(_define.at(LOG_FILE_NAME)).getAbsolutePath();
}

const std::string& CommonCompiler::getSourcePath() const
{
	return _sourcePath.getAbsolutePath();
}
