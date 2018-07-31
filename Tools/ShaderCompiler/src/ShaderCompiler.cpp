#include "precomp.h"
#include "ShaderCompiler.h"
#include "ShaderMetaInfo.h"
#include "Extern/rapidjson/include/rapidjson/document.h"
#include "Extern/rapidjson/include/rapidjson/stringbuffer.h"
#include "Extern/rapidjson/include/rapidjson/prettywriter.h"
#include "Extern/rapidjson/include/rapidjson/fwd.h"
#include "Ash/FileSystem/fs_include.h"
#include "TinyEngine/Engine/EngineDefs.h"

#include "Ash/CommonFunc.h"
#include <list>
#include <regex>
#include <iostream>
#include <thread>
#include <optional>
#include <sstream>
#include "Ash/Container/ArrayStream.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>

static const char JSON_NAME_MAX_THREAD_NUM[] = "thread_number";

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

	if (!writeMetaFile())
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
	_config._output.getAbsolutePath();
	//source
	typedef std::multimap<std::string, std::string>::const_iterator It;
	std::pair<It,It> range = parser.getSplitedCommandLine().equal_range("source");
	for (It it = range.first; it != range.second; ++it)
	{
		_config._source.push_back(it->second);
		_config._source.back().getAbsolutePath();
	}
	//include
	range = parser.getSplitedCommandLine().equal_range("include");
	for (It it = range.first; it != range.second; ++it)
	{
		_config._includePath.push_back(it->second);
		_config._includePath.back().getAbsolutePath();
	}
	_dependenceMgr.setIncludePath(_config._includePath);
	//filter
	_config._filter = parser.getFilter();
	//meta out
	_config._metaOut = parser.getMetaOut();
	_config._metaOut.getAbsolutePath();

	_forceRecompile = parser.getForce();

	_config._intermidiatePath = parser.getIntDir();
	_config._compileRecordJson = Path(std::string(_config._intermidiatePath.getAbsolutePath()) + "\\" + COMPILE_RECORD); //todo: check \\ at end
	_config._dependenceJson = Path(std::string(_config._intermidiatePath.getAbsolutePath()) + "\\" + DEPENDENCE_CACHE_NAME); //todo: check \\ at end
	_config._commandInfoJson = Path(std::string(_config._intermidiatePath.getAbsolutePath()) + "\\" + CMMADNDINFO_CACHE_NAME); //todo: check \\ at end
	_config._compileRecordJson.getAbsolutePath();
	_config._dependenceJson.getAbsolutePath();
	_config._commandInfoJson.getAbsolutePath();

	_dependenceMgr.setRecordFile(_config._dependenceJson);
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
	if (_metaFileIsUpToDate && !_hasCompiledShader)
		return true;

	File file;
	bool result = file.open(_config._metaOut,File::AccessMode::WRITE);
	if(!result)
	{
		DebugString("Open meta output file %s failed", file.getFilePath().getAbsolutePath().c_str());
		return false;
	}

	std::string data;
	data += "#pragma once\n\n";
	data += "#include \"ShaderConstantInfoDef.h\"\n";
	data += "#include \"Ash/TinyAssert.h\"\n\n";
	data += "/***************************************************************************\n";
	data += "* This file is auto generated by ShaderCompiler. Don't modify it manually. *\n";
	data += "****************************************************************************/\n\n";
	
	std::vector<std::pair<Path&, CompileRecord::SubShaderInfo&>> separatedShaderInfo[static_cast<size_t>(ShaderType::COUNT)]; //0 is vs, 1 is ps
	size_t maxLen = 0;

	// Separate ShaderList
	{
		for (CompileResult& hlsl : _shaderList)
		{
			for (CompileRecord::SubShaderInfo& subShader : hlsl._record._output)
			{
				if (subShader._name.length() > maxLen)
					maxLen = subShader._name.length();
				separatedShaderInfo[static_cast<size_t>(subShader._type)].emplace_back(hlsl._fileName, subShader);
			}
		}
		maxLen += 4;
	}

	//Shader Enum
	for(size_t i = 0 ; i < static_cast<size_t>(ShaderType::COUNT); ++i)
	{
		if (i == static_cast<size_t>(ShaderType::VS))
			data += "enum class VertexShader\n{\n";
		else if (i == static_cast<size_t>(ShaderType::PS))
			data += "enum class PixelShader\n{\n";
		for (auto& subShader : separatedShaderInfo[i])
		{
			data += "\t" + toUpper(subShader.second._name) + "," + std::string(maxLen - subShader.second._name.length(), ' ') + "//From: " + subShader.first.getFileName() + "\n";
		}
		if (i == static_cast<size_t>(ShaderType::VS))
			data += "\n\tSHADER_COUNT\n};\n\n";
		else if (i == static_cast<size_t>(ShaderType::PS))
			data += "\n\tSHADER_COUNT\n};\n\n";
	}

	//Local constant
	for (size_t i = 0; i < static_cast<size_t>(ShaderType::COUNT); ++i)
	{
		std::string switchCase;
		std::string getConstantFromName;
		int shaderNum = 0;
		if (i == static_cast<size_t>(ShaderType::VS))
			data += "namespace VsLocalConstant\n{\n";
		else if (i == static_cast<size_t>(ShaderType::PS))
			data += "namespace PsLocalConstant\n{\n";
		bool inShaderElseIsNeeded = false;
		for (auto& subShader : separatedShaderInfo[i])
		{
			if (subShader.second._localParamsInfo.empty())
				continue;

			getConstantFromName += std::string("\t\t") + (inShaderElseIsNeeded ? "else " : "") + "if(shaderName == \"" + toUpper(subShader.second._name) + "\")\n\t\t{\n";
			inShaderElseIsNeeded = true;

			++shaderNum;
			int paramNum = 0;
			data += "\tenum class " + toUpper(subShader.second._name) + "\n\t{\n";
			bool inConstantElseIsNeeded = false;
			for (auto& param : subShader.second._localParamsInfo)
			{
				++paramNum;
				std::string id = std::to_string(shaderNum * 1000 + paramNum);
				data += std::string("\t\t") + toUpper(param._name) + " = " + id + ",\n";
				switchCase += "\t\tcase " + id + ":return {" + std::to_string(param._slot / (sizeof(float) * 4)) + "," + std::to_string(param._size) + "};\n";
				getConstantFromName += std::string("\t\t\t") + (inConstantElseIsNeeded ? "else " : "") + "if (constantName == \"" + toUpper(param._name) + "\") return " + id + ";\n";;
				inConstantElseIsNeeded = true;
			}
			getConstantFromName += "\t\t}\n";
			data += "\t};\n";
		}
		data += "\tinline int getConstantIdFromName(std::string shaderName, std::string constantName)\n\t{\n";
		data += "\t\tshaderName = toUpper(shaderName);\n";
		data += "\t\tconstantName = toUpper(constantName);\n";
		data += getConstantFromName;
		data += "\t\tTinyAssert(false, \"Not found local constant name\");\n";
		data += "\t\treturn -1;";
		data += "\t}\n";

		data += "}\n\n";

		data += std::string() + "inline LocalConstantInfo get" + (i == static_cast<size_t>(ShaderType::VS) ? "Vs" : "Ps") + "LocalParamInfo(int constant)\n{\n";
		data += "\tswitch(constant)\n\t{\n";
		data += switchCase;
		data += "\t\tdefault:\n\t\t\tTinyAssert(false);\n;\t\t\treturn {-1,-1};\n";
		data += "\t}\n}\n\n";
	}

	//Shader Enum to string
	for (size_t i = 0; i < static_cast<size_t>(ShaderType::COUNT); ++i)
	{
		std::string shaderType = (i == static_cast<size_t>(ShaderType::VS) ? "VertexShader" : "PixelShader");
		data += "inline const char* get" + shaderType + "Name(" + shaderType + " shader)\n{\n";
		data += "\tswitch(shader)\n\t{\n";
		for (auto& subShader : separatedShaderInfo[i])
		{
			data += std::string() + "\t\tcase " + shaderType + "::"
				+ toUpper(subShader.second._name) + ": return \"" + toUpper(subShader.second._name) + "\";\n";
		}
		data += "\t\tdefault:\n\t\t\tTinyAssert(false);\n\t\t\treturn nullptr;\n";
		data += "\t}\n}\n\n";
	}

	//Shader string to Enum
	for (size_t i = 0; i < static_cast<size_t>(ShaderType::COUNT); ++i)
	{
		std::string shaderType = (i == static_cast<size_t>(ShaderType::VS) ? "VertexShader" : "PixelShader");
		data += "inline " + shaderType + " get" + shaderType + "FromStr(std::string name)\n{\n";
		data += "\tname = toUpper(name);\n";
		for (auto& subShader : separatedShaderInfo[i])
		{
			data += std::string("\tif (name == \"") + toUpper(subShader.second._name) + "\")return " + shaderType + "::" + toUpper(subShader.second._name) + ";\n";
		}
		data += "\tTinyAssert(false, \"Get shader from name string failed. No matching shader.\");\n\treturn static_cast<" + shaderType + ">(-1);\n}\n\n";
	}

	file.write(data.c_str(), static_cast<int>(data.length()));
	file.setEndOfFile();
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
		CompileRecord::SubShaderInfo info;
		info._outPath = outputFile;
		info._timeStamp = getTimeStamp(outputFile);
		info._name = declear._name;
		ShaderReflectionParser refParser(outputFile);
		info._localParamsInfo = refParser.getParamInfo();
		info._type = declear._shaderType;
		record._output.emplace_back(info);//todo: if output file not exist
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
			_hasCompiledShader = true;
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
		TINY_BREAK_IF(_forceRecompile);
		TINY_BREAK_IF(!_exeIsUpToData);
		TINY_BREAK_IF(!record.has_value());
		TINY_BREAK_IF(record->_timeStamp != getTimeStamp(file));
		bool allOutputUpToDate = true;
		for (const auto& output : record->_output)
		{
			if (getTimeStamp(output._outPath) != output._timeStamp)
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

	bool result = _dependenceMgr.parseDependenceCache();
	result &= readCompileRecord();
	return  result;
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

	ArrayStream is;
	is.initAsInputStream(data.data(), data.size());
	boost::archive::binary_iarchive ia(is);
	ia >> _recordFromLastComp;

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

	ArrayStream is;
	is.initAsInputStream(data.data(), data.size());
	boost::archive::binary_iarchive ia(is);

	uint64_t lastExeTime;
	ia >> lastExeTime;
	_exeIsUpToData = lastExeTime == getTimeStamp(Path::getExePath());

	uint64_t lastMetaOutTime;
	ia >> lastMetaOutTime;
	_metaFileIsUpToDate = lastMetaOutTime == getTimeStamp(_config._metaOut);

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
	bool result = _dependenceMgr.writeDependenceCacheToFile();
	result &= refreshCompileRecord();
	result &= refreshCommandInfoCache();
	return result;
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

	_recordFromLastComp.clear();
	for(auto& result : _shaderList)
	{
		_recordFromLastComp[result._fileName] = result._record;
	}

	std::stringstream ss;
	boost::archive::binary_oarchive oa(ss);
	oa << _recordFromLastComp;
	std::string data = ss.str();
	file.write(data.c_str(), static_cast<int>(data.size()));
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

	std::stringstream ss;
	boost::archive::binary_oarchive oa(ss);
	oa << getTimeStamp(Path::getExePath());
	oa << getTimeStamp(_config._metaOut);
	std::string data = ss.str();
	file.write(data.c_str(), static_cast<int>(data.size()));

	file.setEndOfFile();
	file.close();
	return true;
}
