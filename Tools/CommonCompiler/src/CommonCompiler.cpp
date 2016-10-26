#include "CommonCompiler.h"
#include <fstream>
#include "..\..\..\Engine\ThirdParty\rapidjson\document.h"
#include "..\..\..\Engine\ThirdParty\rapidjson\writer.h"
#include "..\..\..\Engine\ThirdParty\rapidjson\fwd.h"
#include "..\..\..\Engine\ThirdParty\rapidjson\stringbuffer.h"
#include "..\..\..\Engine\Ash\FileSystem\File_Win.h"
using namespace rapidjson;

CommonCompiler::CommonCompiler()
{
}


CommonCompiler::~CommonCompiler()
{
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
	if (!_configFilePath.isFile())
		return false;

	File file(_configFilePath, File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
	if (!file.open())
		return false;

	Document json;
	json.Parse(file.readAll().data());
	if (json.HasParseError())
		return false;

	if (json.HasMember("SourcePath"))
		_sourcePath = json["SourcePath"].GetString();

	if (json.HasMember("SourceFilter"))
		_sourceFilter = json["SourceFilter"].GetString();

	if (json.HasMember("TempFile"))
		_tempFile = json["TempFile"].GetString();

	if (json.HasMember("Dependency"))
	{
		Value& arr = json["Dependency"];
		if (arr.IsString())
			_dependency.push_back(arr.GetString());
		else if (arr.IsArray())
		{
			for (Value& val : arr.GetArray())
				_dependency.push_back(Path(val.GetString()));
		}
	}

	if (json.HasMember("Output"))
		_output = json["Output"].GetString();

	if (json.HasMember("Cmd"))
		_cmd = json["Cmd"].GetString();
	return true;
}


const Path& CommonCompiler::getConfigFilePath() const
{
	return _configFilePath;
}
