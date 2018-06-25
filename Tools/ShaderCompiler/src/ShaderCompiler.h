#pragma once
#include <string>
#include "../../../Engine/Ash/FileSystem/Path_Win.h"
#include <vector>
#include "../../../Engine/Ash/FileSystem/File_Win.h"
#include <map>
#include "../../../Engine/TinyEngine/ThirdParty/rapidjson/document.h"
#include "Ash/CommandLineCfg/CommandLineCfg.h"
#include <atomic>
#include <set>
#include <optional>
#include "ShaderMetaMgr.h"

enum class ShaderType;

class MyCommandLineCfg : public CommandLineCfg
{
public:
	DEFINE_COMMANDLINE_STR(Output, "", "/Output:OutputFile. Give the shader output file path and name");
	DEFINE_COMMANDLINE_STR(Source, "", "/Source:The hlsl file. Give the shader output file path and name");
	DEFINE_COMMANDLINE_STR(Filter, "", "/Filter:The filter which will be used to filter out all the files to be compiled");
	DEFINE_COMMANDLINE_STR(IntDir, "", "/IntDir:Intermidiate directory where compiler will keep the dependecy files and tempory file. All files in IntDir should be managed by compiler, not user.");
};

struct CompileRecord
{
	CompileRecord() = default;
	CompileRecord(const CompileRecord&) = default;
	CompileRecord(CompileRecord&&) = default;
	~CompileRecord() = default;
	CompileRecord& operator=(const CompileRecord&) = default;
	CompileRecord& operator=(CompileRecord&&) = default;
	uint64_t _timeStamp = 0;
	uint64_t _dependTimeStampHash = 0;
	std::vector<std::pair<Path,uint64_t> > _output;
};

struct Config
{
	Path _output;
	Path _source;
	Path _intermediatePath;
	Path _compileRecordJson;
	Path _dependenceJson;
	std::string _filter;
	
	int _threadNumber = 6;

	Path _intermidiatePath;
};

class ShaderCompiler
{
public:
	ShaderCompiler();
	~ShaderCompiler();

	int run(const std::string& cmdLine);
protected:
	Config _config;

	std::map<Path, CompileRecord> _recordFromLastComp;

	struct CompileResult
	{
		CompileResult(const Path& fileName) :_fileName(fileName) {}
		Path _fileName;
		bool _succeed = false;
		CompileRecord _record;
	};
	std::vector<CompileResult> _shaderList;

	std::atomic <uint32_t> _shaderListIndex = 0;
	std::atomic<bool> _hasCompillingError = false;
	ShaderMetaMgr _dependenceMgr;


	bool parseArg(const std::string& cmdLine);
	bool parseConfig();

	bool fillShaderList();
	bool allocThreadAndDoWork();
	std::optional<CompileRecord> compileShader(const Path& file);
	bool doCompile(const Path& sourceFile, ShaderType shaderType, const std::string& entry, const std::vector<std::string> defines, const Path& output);

	bool threadWorker();
	bool checkTimeStampHashOfAllDependence(const Path& file, uint64_t depdenceTimeStampHash);
	uint64_t calcTimeStampHash(const DependenceInfo& depInfo);
	uint64_t calcTimeStampHashImp(const DependenceInfo& depInfo, std::set<Path>& alreadyCheckedList);
	bool checkCompileRecordIsUpToDate(const Path& file, const std::optional<CompileRecord>& record);

	bool readCache();
	bool readCompileRecord();
	std::optional<CompileRecord> getCompileRecord(const Path& file);
	bool writeCache();
	bool refreshCompileRecord();
};

