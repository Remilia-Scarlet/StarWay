#define _CRT_SECURE_NO_WARNINGS
#include "CommonCompiler.h"
#include <fstream>
#include "Engine\ThirdParty\rapidjson\document.h"
#include "Engine\ThirdParty\rapidjson\writer.h"
#include "Engine\ThirdParty\rapidjson\fwd.h"
#include "Engine\ThirdParty\rapidjson\stringbuffer.h"
#include "Engine\ThirdParty\rapidjson\prettywriter.h"
#include "Engine\Ash\FileSystem\File_Win.h"
#include "Engine\TinyEngine\Engine\EngineDefs.h"

#include "Engine\Ash\CommonFunc.h"
#include <list>
#include <stack>
using namespace rapidjson;

const char* SOURCE_PATH_NAME = "$(SourcePath)";
const char* SOURCE_FILTER_NAME = "$(SourceFilter)";
const char* TIME_STAMP_FILE_NAME = "$(TimeStampFile)";
const char* DEPENDENCY_NAME = "$(Dependency)";
const char* OUTPUT_NAME = "$(Output)";
const char* COMPILE_EXE_NAME = "$(CompileExe)";
const char* COMPILE_PARAM_NAME = "$(CompileParam)";

const char* SOURCE_FILE_NAME = "$(SourceName)";
const char* SOURCE_EXT_NAME = "$(SourceExt)";
const char* SOURCE_FULL_NAME = "$(SourceFullName)";
const char* SUB_FOLDER_NAME = "$(SubFolder)";

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

	if (!checkPath())
		return 4;

	if (!compile())
		return 5;
	
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
	{
		DebugString("Can't open json config file:\n%s", _configFilePath.c_str());
		return false;
	}

	Document json;
	std::vector<char> data = file.readAll();
	json.Parse(data.data(),data.size());
	auto asd = json.GetParseError();
	if (json.GetParseError())
	{
		DebugString("Can't parse json config file:\n%s", _configFilePath.c_str());
		return false;
	}

	if (json.HasMember(SOURCE_PATH_NAME))
		_define[SOURCE_PATH_NAME] = json[SOURCE_PATH_NAME].GetString();

	if (json.HasMember(SOURCE_FILTER_NAME))
		_define[SOURCE_FILTER_NAME] = json[SOURCE_FILTER_NAME].GetString();

	if (json.HasMember(TIME_STAMP_FILE_NAME))
		_define[TIME_STAMP_FILE_NAME] = json[TIME_STAMP_FILE_NAME].GetString();

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

	if (json.HasMember(COMPILE_EXE_NAME))
		_define[COMPILE_EXE_NAME] = json[COMPILE_EXE_NAME].GetString();

	if (json.HasMember(COMPILE_PARAM_NAME))
		_define[COMPILE_PARAM_NAME] = json[COMPILE_PARAM_NAME].GetString();
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
					if (it2 == it1)//loop define
					{
						DebugString("Find loop define in config file");
						return false;
					}
					loop = true;
					replaceSubstr(val1, key2, val2);
				}
			}
		}
		if (!loop)
			break;
	}
	if (loopRemainTimes == 0)
	{
		DebugString("Find loop define in config file");
		return false;
	}
	return true;
}

bool CommonCompiler::checkPath()
{
	_sourcePath = _define[SOURCE_PATH_NAME];
	if (!_sourcePath.isDirectory())
	{
		DebugString("Can't open source path:\n%s", _define[SOURCE_PATH_NAME].c_str());
		return false;
	}

	if (!_logFile.open(_define[TIME_STAMP_FILE_NAME]))
	{
		DebugString("Can't open time stamp file:\n%s", _define[TIME_STAMP_FILE_NAME]);
		return false;
	}
	std::vector<char> data = _logFile.readAll();
	_logFile.close();
	_logDoc.Parse(data.data(), data.size());
	if (_logDoc.HasParseError())
		_logDoc.SetObject();
	flushLog();

	_cmdExe = _define[COMPILE_EXE_NAME];
	if (!_cmdExe.isFile())
	{
		DebugString("Can't find [%s]", _define[COMPILE_EXE_NAME].c_str());
		return false;
	}

	return true;
}

bool CommonCompiler::compile()
{
	std::stack<std::string> stack;
	stack.push(".");
	std::string filter = _define[SOURCE_FILTER_NAME];
	bool ret = true;
	while (!stack.empty())
	{
		std::string path = stack.top();
		stack.pop();
		WIN32_FIND_DATA att;
		HANDLE hFind = FindFirstFile((_sourcePath.getAbsolutePath() + "\\" + path + "\\" + filter).c_str(), &att);
		if (hFind == INVALID_HANDLE_VALUE)
			continue;
		do
		{
			if (strcmp(att.cFileName, ".") == 0 || strcmp(att.cFileName, "..") == 0)
				continue;

			if (att.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				stack.push(path + "\\" + att.cFileName);
			else
			{
				std::string fullPath = _sourcePath.getAbsolutePath() + "\\" + path + "\\" + att.cFileName;
				bool result = compileFile(att, 
					Path(
						fullPath,
						"",
						true,
						"",
						true,
						false,
						true,
						false
					), path);
				ret = ret && result;
			}
		} while (::FindNextFile(hFind, &att));
		FindClose(hFind);
	}
	return ret;
}

bool CommonCompiler::compileFile(const WIN32_FIND_DATA& info, const Path& path, const std::string& subFolder)
{
	auto& key = path.getRelativePath();
	auto memberIt = _logDoc.FindMember(key.c_str());
	if (memberIt != _logDoc.MemberEnd())
	{
		Value& logItem = memberIt->value;
		if (logItem.IsArray() && logItem.Size() == 2)
		{
			auto arr = logItem.GetArray();
			DWORD l = arr[0].GetUint();
			DWORD h = arr[1].GetUint();
			if (info.ftLastWriteTime.dwHighDateTime == h && info.ftLastWriteTime.dwLowDateTime == l)
				return true;
		}
	}
	std::string cmd = fixRuntimeDefine(info, _define[COMPILE_PARAM_NAME], subFolder);
	std::string outPut = fixRuntimeDefine(info, _define[OUTPUT_NAME], subFolder);
	Path outPath = outPut;
	if (!outPath.getParentDirectory().createDirectory())
	{
		DebugString("Can't open output file:\n%s", outPath.getAbsolutePath().c_str());
		return false;
	}
	if (!callCmdAndWaitFinish(info, cmd, subFolder))
	{
		_logDoc.EraseMember(key.c_str());
		return false;
	}

	WIN32_FIND_DATA att;
	HANDLE hFind = FindFirstFile(path.getAbsolutePath().c_str(), &att);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		_logDoc.EraseMember(key.c_str());
		return false;
	}

	memberIt = _logDoc.FindMember(key.c_str());
	if (memberIt != _logDoc.MemberEnd())
		memberIt->value.SetArray();
	else
		_logDoc.AddMember(Value(key.c_str(), (SizeType)key.length(), _logDoc.GetAllocator()).Move(), Value(kArrayType).Move(), _logDoc.GetAllocator());

	Value& arr = _logDoc[key.c_str()];
	arr.PushBack((unsigned int)att.ftLastWriteTime.dwLowDateTime, _logDoc.GetAllocator());
	arr.PushBack((unsigned int)att.ftLastWriteTime.dwHighDateTime, _logDoc.GetAllocator());
	flushLog();
	return true;
}

bool CommonCompiler::callCmdAndWaitFinish(const WIN32_FIND_DATA& info, const std::string& cmd, const std::string& subFolder)
{
	DebugString("Calling command:\n\"%s\" %s\n", _cmdExe.getAbsolutePath().c_str(),  cmd.c_str());
	SECURITY_ATTRIBUTES saAttr;
	saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
	saAttr.bInheritHandle = TRUE;
	saAttr.lpSecurityDescriptor = NULL;
	HANDLE childOutRd = NULL;
	HANDLE childOutWr = NULL;
	if (!CreatePipe(&childOutRd, &childOutWr, &saAttr, 0))
	{
		DebugString("Create pipe for compiler failed");
		return false;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.hStdError = childOutWr;
	si.hStdOutput = childOutWr;
	si.dwFlags |= STARTF_USESTDHANDLES;

	ZeroMemory(&pi, sizeof(pi));

	char* cmdNotConst = new char[1 + cmd.length() + 1];
	cmdNotConst[0] = ' ';
	memcpy(cmdNotConst + 1, cmd.c_str(), cmd.length() + 1);
	// Start the child process. 
	if (!CreateProcess(
		_cmdExe.getAbsolutePath().c_str(),
		cmdNotConst,      
		NULL,          
		NULL, 
		TRUE,     
		0,     
		NULL, 
		NULL,        
		&si,      
		&pi)    
		)
	{
		printf("CreateProcess failed (%d).\n", GetLastError());
		return false;
	}
	delete[] cmdNotConst;
	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD exitCode;
	if (!GetExitCodeProcess(pi.hProcess, &exitCode))
		exitCode = GetLastError();

	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	CloseHandle(childOutWr);

	const int bufferSize = 1000;
	char* pipeBuffer = new char[bufferSize + 1];
	while (1)
	{
		DWORD readBytes = 0;
		BOOL result = ReadFile(childOutRd, pipeBuffer, bufferSize, &readBytes, NULL);
		pipeBuffer[readBytes] = 0;
		if (result && readBytes)
			DebugString(pipeBuffer);
		else
			break;
	}
	delete[] pipeBuffer;

	DebugString("");
	return exitCode == 0;
}


bool CommonCompiler::flushLog()
{
	StringBuffer buffer;
	PrettyWriter<StringBuffer> writer(buffer);
	_logDoc.Accept(writer);
	const char* fileData = buffer.GetString();
	_logFile.open(_logFile.getFilePath());
	_logFile.seek(0);
	_logFile.write(fileData, (int)buffer.GetLength());
	_logFile.setEndOfFile();
	_logFile.close();
	return true;
}

std::string CommonCompiler::fixRuntimeDefine(const WIN32_FIND_DATA& info, const std::string& def, const std::string& subFolder)
{
	_runtimeDefine[SUB_FOLDER_NAME] = subFolder;
	std::string& file = (_runtimeDefine[SOURCE_FULL_NAME] = info.cFileName);
	size_t pos = file.find_last_of('.');
	if (pos == -1)
	{
		_runtimeDefine[SOURCE_FILE_NAME] = file;
		_runtimeDefine[SOURCE_EXT_NAME] = "";
	}
	else 
	{
		_runtimeDefine[SOURCE_FILE_NAME] = file.substr(0, pos);
		_runtimeDefine[SOURCE_EXT_NAME] = file.substr(pos + 1);
	}
	std::string copyDef = def;
	bool loop = false;
	do 
	{
		loop = false;
		for (auto& pair : _runtimeDefine)
		{
			if (copyDef.find(pair.first) != -1)
			{
				loop = true;
				replaceSubstr(copyDef, pair.first, pair.second);
			}
		}
	} while (loop);
	

	return copyDef;
}
