#include "precomp.h"
#include "ShaderCompiler.h"
#include "ShaderMetaInfo.h"
#include "Extern/rapidjson/include/rapidjson/document.h"
#include "Extern/rapidjson/include/rapidjson/stringbuffer.h"
#include "Extern/rapidjson/include/rapidjson/prettywriter.h"
#include "Extern/rapidjson/include/rapidjson/fwd.h"
#include "Engine/Ash/FileSystem/fs_include.h"
#include "Engine/TinyEngine/Engine/EngineDefs.h"

#include "Engine/Ash/CommonFunc.h"
#include <list>
#include <regex>
#include <iostream>
#include <thread>
#include <optional>

static const char JSON_NAME_MAX_THREAD_NUM[] = "thread_number";
static const char JSON_NAME_TIME_STAMP[] = "time_stamp";
static const char JSON_NAME_DEPENDENT_HASH[] = "depend_time_stamp_hash";
static const char JSON_NAME_OUTPUT_FILES[] = "output_files";
static const char JSON_NAME_META_FILE_TIME[] = "meta_file_timestamp";
static const char JSON_NAME_FILE_NAME[] = "file_name";
static const char JSON_NAME_EXE_TIME[] = "exe_timestamp";

static const char CONFIG_JSON[] = "game:config.json";

static const char COMPILE_RECORD[] = "_shader_compile_record.tmp";
static const char DEPENDENCE_CACHE_NAME[] = "_shader_dependence.tmp";
static const char CMMADNDINFO_CACHE_NAME[] = "_shader_command.tmp";

using namespace rapidjson;

ShaderCompiler::ShaderCompiler()
{
}


ShaderCompiler::~ShaderCompiler()
{
}

int ShaderCompiler::run(const std::string& cmdLine)
{
	//读取配置
	if (!parseArg(cmdLine) || !parseConfig())
		return -1;

	//查找shader文件夹->hlsl list
	if (!fillShaderList())
		return 1;

	if (!readCache())
		return 1;

	if (!allocThreadAndDoWork())
		return 1;

	if (!writeCache())
		return 1;

	return 0;
}

bool ShaderCompiler::parseArg(const std::string& cmdLine)
{
	MyCommandLineCfg parser;
	bool result = parser.init(cmdLine.c_str());;
	if (!result)
	{
		DebugString("Parsing command line error, use /h for help.");
		return false;
	}
	//output path
	_config._output = parser.getOutput();
	//source
	typedef std::multimap<std::string, std::string>::const_iterator It;
	std::pair<It,It> range = parser.getSplitedCommandLine().equal_range("source");
	for (It it = range.first; it != range.second; ++it)
	{
		_config._source.push_back(it->second);
	}
	//include
	range = parser.getSplitedCommandLine().equal_range("include");
	for (It it = range.first; it != range.second; ++it)
	{
		_config._includePath.push_back(it->second);
	}
	_dependenceMgr.setIncludePath(_config._includePath);
	//filter
	_config._filter = parser.getFilter();
	//meta out
	_config._metaOut = parser.getMetaOut();

	_config._intermidiatePath = parser.getIntDir();
	_config._compileRecordJson = Path(std::string(_config._intermidiatePath.getAbsolutePath()) + "\\" + COMPILE_RECORD); //todo: check \\ at end
	_config._dependenceJson = Path(std::string(_config._intermidiatePath.getAbsolutePath()) + "\\" + DEPENDENCE_CACHE_NAME); //todo: check \\ at end
	_config._commandInfoJson = Path(std::string(_config._intermidiatePath.getAbsolutePath()) + "\\" + CMMADNDINFO_CACHE_NAME); //todo: check \\ at end
	return true;
}

bool ShaderCompiler::parseConfig()
{
	File configFile;
	configFile.open(CONFIG_JSON, File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
	if (!configFile.isOpened())
	{
		DebugString("Can't open %s", CONFIG_JSON);
		return false;
	}

	std::vector<char> data = configFile.readAll();
	if (data.empty())
	{
		DebugString("Can't open %s", CONFIG_JSON);
		return false;
	}

	Document jsonDoc;
	jsonDoc.Parse<kParseStopWhenDoneFlag>(data.data());
	if (jsonDoc.HasParseError() || !jsonDoc.IsObject())
	{
		DebugString("Parsing %s error",CONFIG_JSON);
		return false;
	}

	for(Value::MemberIterator it = jsonDoc.MemberBegin(); it != jsonDoc.MemberEnd(); ++it)
	{
		if(it->name == JSON_NAME_MAX_THREAD_NUM)
		{
			TinyAssert(it->value.GetType() == kNumberType);
			_config._threadNumber = it->value.GetInt();
		}
	}
	return true;
}

bool ShaderCompiler::fillShaderList()
{
	for(auto& shaderPath : _config._source)
	{
		if (!shaderPath.isDirectory())
		{
			std::cout << "\"" << shaderPath.getAbsolutePath() << "\" does not exist." << std::endl;
			continue;
		}
		std::list<Path> allFiles = shaderPath.getFileList(_config._filter);
		for (const Path& pa : allFiles)
		{
			_shaderList.emplace_back(pa.getRelativePath());
		}
	}
	
	return true;
}

bool ShaderCompiler::allocThreadAndDoWork()
{
	std::vector<std::thread> arr;
	for (int i = 0; i < _config._threadNumber; ++i)
	{
		arr.emplace_back(std::bind(&ShaderCompiler::threadWorker, this));
	}
	for (std::thread& th : arr)
	{
		th.join();
	}
	return !_hasCompillingError.load();
}

bool ShaderCompiler::writeMetaFile()
{
	return true;
}

std::optional<CompileRecord> ShaderCompiler::compileShader(const Path& file)
{
	CompileRecord record;
	ShaderMetaInfo shaderMeta = _dependenceMgr.getMetaInfo(file);
	record._timeStamp = getTimeStamp(file);
	record._dependTimeStampHash = calcTimeStampHash(_dependenceMgr.getDependent(file));
	for (const ShaderDeclear& declear : shaderMeta._declears) //todo: meta head file for c++ not include
	{
		Path outputFile = declear.calculateOutputFileName(_config._output);
		bool result = doCompile(file, declear._shaderType, declear._entry, declear._defines, outputFile);
		if (!result)
		{
			DebugString("compiling shader %s failed with entry: %s, declear: %s", file.getFileName().c_str(), declear._entry.c_str(), outputFile.getFileNameWithoutExt().c_str());
			return {};
		}
		record._output.emplace_back(outputFile, getTimeStamp(outputFile));//todo: if output file not exist
	}
	return { record };
}

bool ShaderCompiler::doCompile(const Path& sourceFile, ShaderType shaderType, const std::string& entry, const std::vector<std::string> defines, const Path& output)
{
	std::string fileName = sourceFile.getFileName();
	std::string shaderTypeStr = (shaderType == ShaderType::VS ? "vs" : "ps");

	Path fxcPath("game:fxc.exe");
	std::string cmd = fxcPath.getAbsolutePath();

	output.getParentDirectory().createDirectory();

	std::string param = 
		  " /T " + shaderTypeStr + "_5_0" // /T vs_5_0 
		+ " /E " + entry  // /E main
		+ " /O3"
		+ " /Zi"
		+ " /Fo \"" + output.getAbsolutePath() + "\""
		+ " /nologo";

	for (auto& path : _config._includePath)
	{
		param += " /I \"" + path.getAbsolutePath() + " \"";
	}

	param += " /D_SHADER=1";
	for(auto& def : defines)
	{
		param += " /D " + def + "=1";
	}

	param += " \"" + sourceFile.getAbsolutePath() + "\" ";

	STARTUPINFO si;
	memset(&si, 0, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = 0;
	si.wShowWindow = SW_HIDE;
	PROCESS_INFORMATION pi;
	BOOL result = CreateProcess(
		cmd.data(),
		const_cast<char*>(param.c_str()),
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi
	);
	WaitForSingleObject(pi.hProcess, INFINITE);

	DWORD exitCode = 1;
	if (!GetExitCodeProcess(pi.hProcess, &exitCode))
	{
		exitCode = 1;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	return exitCode == 0;
}

bool ShaderCompiler::threadWorker()
{
	do
	{
		uint32_t id = _shaderListIndex++;//atomic add
		if (id >= _shaderList.size() || _hasCompillingError.load())
			return true;

		CompileResult& compile = _shaderList[id];
		Path filePath(compile._fileName);

		std::optional<CompileRecord> compileRecord = getCompileRecord(filePath);
		if (checkCompileRecordIsUpToDate(filePath, compileRecord))
		{
			// already up to date
			compile._succeed = true;
			compile._record = *compileRecord;
		}
		else
		{
			std::optional<CompileRecord> result = compileShader(filePath);
			if(result.has_value())
			{
				// compile succeed
				compile._succeed = true;
				compile._record = std::move(*result);
			}
			else
			{
				// compile failed
				compile._succeed = false;
				_hasCompillingError = true;
				break;
			}
		}
	} while (true);
	return false;
}

bool ShaderCompiler::checkTimeStampHashOfAllDependence(const Path& file, uint64_t depdenceTimeStampHash)
{
	if (!file.isFile())
		return false;
	const DependenceInfo& depInfo = _dependenceMgr.getDependent(file);
	uint64_t hash = calcTimeStampHash(depInfo);
	return hash == depdenceTimeStampHash;
}

uint64_t ShaderCompiler::calcTimeStampHash(const DependenceInfo& depInfo)
{
	std::set<Path> alreadyCheckedList;
	return calcTimeStampHashImp(depInfo, alreadyCheckedList);
}

uint64_t ShaderCompiler::calcTimeStampHashImp(const DependenceInfo& depInfo, std::set<Path>& alreadyCheckedList)
{
	uint64_t hash = 14695981039346656037ULL;

	static std::function<void(uint64_t&, uint64_t)> hashAddVal = [](uint64_t& hashInput, uint64_t val) {
		constexpr uint64_t prime = 1099511628211ULL;
		hashInput ^= val;
		hashInput *= prime;
	};
	
	for (const Path& file : depInfo._dependences)
	{
		if (alreadyCheckedList.find(file) == alreadyCheckedList.end())
		{
			alreadyCheckedList.insert(file);
			hashAddVal(hash, getTimeStamp(file));
			if(file.isFile())
			{
				const DependenceInfo& myDependence = _dependenceMgr.getDependent(file);
				uint64_t myDepthHash = calcTimeStampHashImp(myDependence, alreadyCheckedList);
				if(myDepthHash != 0)
					hashAddVal(hash, myDepthHash);
			}

		}
	}
	return hash;
}

bool ShaderCompiler::checkCompileRecordIsUpToDate(const Path& file, const std::optional<CompileRecord>& record)
{
	do
	{
		TINY_BREAK_IF(!_exeIsUpToData);
		TINY_BREAK_IF(!record.has_value());
		TINY_BREAK_IF(record->_timeStamp != getTimeStamp(file));
		bool allOutputUpToDate = true;
		for (const std::pair<Path,uint64_t>& output : record->_output)
		{
			if (getTimeStamp(output.first) != output.second)
			{
				allOutputUpToDate = false;
				break;
			}
		}
		TINY_BREAK_IF(!allOutputUpToDate);
		TINY_BREAK_IF(!checkTimeStampHashOfAllDependence(file, record->_dependTimeStampHash));
		return true;
	} while (false);
	return false;
}

bool ShaderCompiler::readCache()
{
	readCommandInfoCache();
	if (!_exeIsUpToData)
		return true;
	return _dependenceMgr.parseDependenceCache(_config._dependenceJson)
		&& readCompileRecord();
}

bool ShaderCompiler::readCompileRecord()
{
	File file;
	file.open(_config._compileRecordJson.getAbsolutePath(), File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
	if (!file.isOpened())
	{
		return true;
	}

	std::vector<char> data = file.readAll();
	if (data.empty())
		return true;

	rapidjson::Document doc;
	doc.Parse<kParseStopWhenDoneFlag>(data.data());
	if (doc.HasParseError() || doc.GetType() != rapidjson::Type::kObjectType)
	{
		file.close();
		DeleteFile(file.getFilePath().getAbsolutePath().c_str());
		return true;
	}

	for (Value::MemberIterator itDoc = doc.MemberBegin(); itDoc != doc.MemberEnd(); ++itDoc)
	{
		CompileRecord record;
		Path fileName = Path(itDoc->name.GetString());

		Value& recordVal = itDoc->value;
		TinyAssert(recordVal.IsObject());

		for (Value::MemberIterator itRecord = recordVal.MemberBegin(); itRecord != recordVal.MemberEnd(); ++itRecord)
		{
			if (itRecord->name == JSON_NAME_TIME_STAMP)
			{
				TinyAssert(itRecord->value.IsUint64());
				record._timeStamp = itRecord->value.GetUint64();
			}
			else if (itRecord->name == JSON_NAME_DEPENDENT_HASH)
			{
				TinyAssert(itRecord->value.IsUint64());
				record._dependTimeStampHash = itRecord->value.GetUint64();
			}
			else if (itRecord->name == JSON_NAME_OUTPUT_FILES)
			{
				TinyAssert(itRecord->value.IsArray());
				for (const auto& outputVal : itRecord->value.GetArray())
				{
					TinyAssert(outputVal.IsObject() && outputVal.MemberCount() == 1);
					const Value& outName = outputVal.MemberBegin()->name;
					const Value& outTimeStamp = outputVal.MemberBegin()->value;
					TinyAssert(outTimeStamp.IsUint64());
					record._output.emplace_back(outName.GetString(), outTimeStamp.GetUint64());
				}
			}
		}
		_recordFromLastComp.emplace(fileName, record);
	}
	return true;
}

bool ShaderCompiler::readCommandInfoCache()
{
	File file;
	file.open(_config._commandInfoJson.getAbsolutePath(), File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
	if (!file.isOpened())
	{
		return true;
	}

	std::vector<char> data = file.readAll();
	if (data.empty())
		return true;

	rapidjson::Document doc;
	doc.Parse<kParseStopWhenDoneFlag>(data.data());
	if (doc.HasParseError() || doc.GetType() != rapidjson::Type::kObjectType)
	{
		file.close();
		DeleteFile(file.getFilePath().getAbsolutePath().c_str());
		return true;
	}
	for (Value::MemberIterator itDoc = doc.MemberBegin(); itDoc != doc.MemberEnd(); ++itDoc)
	{
		if(itDoc->name.GetString() == JSON_NAME_META_FILE_TIME)
		{
			TinyAssert(itDoc->value.IsInt64());
			_metaFileIsUpToDate = static_cast<uint64_t>(itDoc->value.GetInt64()) == getTimeStamp(_config._metaOut.getAbsolutePath());
		}
		else if(itDoc->name.GetString() == JSON_NAME_EXE_TIME)
		{
			TinyAssert(itDoc->value.IsInt64());
			_exeIsUpToData = static_cast<uint64_t>(itDoc->value.GetInt64()) == getTimeStamp(Path::getExePath());
		}
	}
	return true;
}

std::optional<CompileRecord> ShaderCompiler::getCompileRecord(const Path & file)
{
	std::map<Path, CompileRecord>::iterator it = _recordFromLastComp.find(file);
	if (it != _recordFromLastComp.end())
		return { it->second };

	return std::nullopt;
}

bool ShaderCompiler::writeCache()
{
	return _dependenceMgr.writeDependenceCacheToFile()
		&& refreshCompileRecord()
		&& refreshCommandInfoCache();
}

bool ShaderCompiler::refreshCompileRecord()
{
	File file;
	file.open(_config._compileRecordJson.getAbsolutePath(), File::AccessMode::WRITE);
	if (!file.isOpened())
	{
		DebugString("Open compile record file [%s] failed", _config._compileRecordJson.getAbsolutePath().c_str());
		return false;
	}

	Document doc(kObjectType);
	for (const CompileResult& result : _shaderList)
	{
		Value compileRecord(kObjectType);
		compileRecord.AddMember(JSON_NAME_TIME_STAMP, result._record._timeStamp, doc.GetAllocator());
		compileRecord.AddMember(JSON_NAME_DEPENDENT_HASH, result._record._dependTimeStampHash, doc.GetAllocator());
		Value outputList(kArrayType);
		for (const std::pair<Path, uint64_t>& arrItem : result._record._output)
		{
			Value outFile(kObjectType);
			const std::string& relaPath = arrItem.first.getRelativePath();
			outFile.AddMember(Value().SetString(relaPath.c_str(), static_cast<SizeType>(relaPath.length())).Move(), arrItem.second, doc.GetAllocator());
			outputList.PushBack(outFile, doc.GetAllocator());
		}
		compileRecord.AddMember(JSON_NAME_OUTPUT_FILES, outputList, doc.GetAllocator());

		const std::string& outPathStr = result._fileName.getRelativePath();
		doc.AddMember(Value().SetString(outPathStr.c_str(), static_cast<SizeType>(outPathStr.length())).Move(), compileRecord, doc.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	bool formatScceed = doc.Accept(writer);
	TinyAssert(formatScceed);
	file.write(buffer.GetString(), static_cast<int>(buffer.GetLength()));
	file.setEndOfFile();
	file.close();
	return true;
}

bool ShaderCompiler::refreshCommandInfoCache()
{
	File file;
	file.open(_config._commandInfoJson.getAbsolutePath(), File::AccessMode::WRITE);
	if (!file.isOpened())
	{
		DebugString("Open compile record file [%s] failed", _config._commandInfoJson.getAbsolutePath().c_str());
		return false;
	}

	Document doc(kObjectType);
	//Meta file record
	doc.AddMember(JSON_NAME_META_FILE_TIME, getTimeStamp(_config._metaOut.getAbsolutePath()), doc.GetAllocator());

	//ShaderCompiler timestamp
	doc.AddMember(JSON_NAME_EXE_TIME, getTimeStamp(Path::getExePath()), doc.GetAllocator());
	
	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	bool formatScceed = doc.Accept(writer);
	TinyAssert(formatScceed);
	file.write(buffer.GetString(), static_cast<int>(buffer.GetLength()));
	file.setEndOfFile();
	file.close();
	return true;
}
