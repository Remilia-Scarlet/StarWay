#pragma once
#include <string>
#include "Engine/Ash/FileSystem/fs_include.h"
#include <vector>
#include <map>
#include "Ash/CommandLineCfg/CommandLineCfg.h"
#include <atomic>
#include <set>
#include <optional>
#include "ShaderMetaMgr.h"
#include "ShaderReflectionInfo.h"

enum class ShaderType;

class MyCommandLineCfg : public CommandLineCfg
{
public:
	DEFINE_COMMANDLINE_STR(Output, "", "/Output:<OutputFile>. Give the shader output file path and name");
	DEFINE_COMMANDLINE_STR(Source, "", "/Source:<SourceFile> The hlsl file. Give the shader output file path and name");
	DEFINE_COMMANDLINE_STR(Filter, "", "/Filter:<*.*> The filter which will be used to filter out all the files to be compiled");
	DEFINE_COMMANDLINE_STR(IntDir, "", "/IntDir:<Directory> Intermidiate directory where compiler will keep the dependecy files and tempory file. All files in IntDir should be managed by compiler, not user.");
	DEFINE_COMMANDLINE_STR(Include, "", "/Include:<Directory> Additional include path. ");
	DEFINE_COMMANDLINE_STR(MetaOut, "", "/MetaOut:<MetaOutFilePath> The .h file that contains all shader meta info");
	DEFINE_COMMANDLINE_BOOL(Force, "/Force force recompile");
};

struct CompileRecord
{
	CompileRecord() = default;
	CompileRecord(const CompileRecord&) = default;
	CompileRecord(CompileRecord&&) = default;
	~CompileRecord() = default;
	CompileRecord& operator=(const CompileRecord&) = default;
	CompileRecord& operator=(CompileRecord&&) = default;

	uint64_t _timeStamp = 0; //hlsl timestamp
	uint64_t _dependTimeStampHash = 0; //all dependeces time stamp hash
	struct SubShaderInfo
	{
		std::string _name; //the name that define inside shader For example default_material
		Path _outPath; // the output file. For example default_material.ps.cso
		uint64_t _timeStamp; // output file timestamp
		std::vector<ParamInfo> _localParamsInfo;//local params.

		friend class boost::serialization::access;
		template<class Archive>
		void serialize(Archive & ar, const unsigned int version)
		{
			ar & _name;
			ar & _outPath;
			ar & _timeStamp;
			ar & _localParamsInfo;
		}
	};
	std::vector<SubShaderInfo> _output;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _timeStamp;
		ar & _dependTimeStampHash;
		ar & _output;
	}
};

struct Config
{
	Path _output;
	std::vector<Path> _source;
	std::vector<Path> _includePath;
	Path _intermediatePath;
	Path _compileRecordJson;
	Path _dependenceJson;
	Path _commandInfoJson;
	std::string _filter;
	Path _metaOut;
	
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
		CompileResult() = default;
		CompileResult(const Path& fileName) :_fileName(fileName) {}
		Path _fileName;
		bool _succeed = false;
		CompileRecord _record;
	};
	std::vector<CompileResult> _shaderList;

	std::atomic <uint32_t> _shaderListIndex = 0;
	std::atomic<bool> _hasCompillingError = false;
	ShaderMetaMgr _dependenceMgr;
	bool _exeIsUpToData = false;
	bool _forceRecompile = false;
	uint64_t _metaFileIsUpToDate = 0;



	bool parseArg(const std::string& cmdLine);
	bool parseConfig();

	bool fillShaderList();
	bool allocThreadAndDoWork();
	bool writeMetaFile();
	std::optional<CompileRecord> compileShader(const Path& file);
	bool doCompile(const Path& sourceFile, ShaderType shaderType, const std::string& entry, const std::vector<std::string> defines, const Path& output);

	bool threadWorker();
	bool checkTimeStampHashOfAllDependence(const Path& file, uint64_t depdenceTimeStampHash);
	uint64_t calcTimeStampHash(const DependenceInfo& depInfo);
	uint64_t calcTimeStampHashImp(const DependenceInfo& depInfo, std::set<Path>& alreadyCheckedList);
	bool checkCompileRecordIsUpToDate(const Path& file, const std::optional<CompileRecord>& record);

	bool readCache();
	bool readCompileRecord();
	bool readCommandInfoCache();
	std::optional<CompileRecord> getCompileRecord(const Path& file);
	bool writeCache();
	bool refreshCompileRecord();
	bool refreshCommandInfoCache();
};

