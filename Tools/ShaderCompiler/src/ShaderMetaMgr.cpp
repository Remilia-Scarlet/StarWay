#include "precomp.h"
#include "ShaderMetaMgr.h"
#include <Extern/rapidjson/include/rapidjson/document.h>
#include <Extern/rapidjson/include/rapidjson/stringbuffer.h>
#include <Extern/rapidjson/include/rapidjson/prettywriter.h>
#include <TinyEngine/Engine/EngineDefs.h>
#include "FileSystem/fs_include.h"
#include <fstream>
#include <iostream>
#include "CommonStateMachine/StateMachine.h"
#include <regex>
using namespace rapidjson;

static const char JSON_NAME_TIME_STAMP[] = "time_stamp";
static const char JSON_NAME_DEPENDENCE[] = "dependence";
static const char JSON_NAME_PARAMS[] = "params";
static const char JSON_NAME_DECLEARATION[] = "declearation";
static const char JSON_NAME_PARAM_NAME[] = "name";
static const char JSON_NAME_PARAM_SLOT[] = "slot";
static const char JSON_NAME_SHADER_NAME[] = "name";
static const char JSON_NAME_SHADER_TYPE[] = "type";
static const char JSON_NAME_SHADER_ENTRY[] = "entry";
static const char JSON_NAME_SHADER_DEFINES[] = "defines";


ShaderMetaMgr::ShaderMetaMgr()
{
}


ShaderMetaMgr::~ShaderMetaMgr()
{
}

bool ShaderMetaMgr::parseDependenceCache(const Path& recordFileName)
{
	_recordFileName = recordFileName;

	File file;
	file.open(_recordFileName.getAbsolutePath(), File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
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
		Path fileName = itDoc->name.GetString();
		Value& oneItem = itDoc->value;

		MetaInfoMapItem& mapItem = _dependence[fileName];
		mapItem._otherInfo._status = OtherInfo::Ready;

		for (Value::MemberIterator itOneItem = oneItem.MemberBegin(); itOneItem != oneItem.MemberEnd(); ++itOneItem)
		{
			if (itOneItem->name == JSON_NAME_TIME_STAMP)
			{
				TinyAssert(itOneItem->value.IsUint64());
				mapItem._dependdInfo._timeStamp = itOneItem->value.GetUint64();
			}
			else if (itOneItem->name == JSON_NAME_DEPENDENCE)
			{
				TinyAssert(itOneItem->value.IsArray());
				std::vector<Path>& depVec = mapItem._dependdInfo._dependences;
				for (const auto& depVal : itOneItem->value.GetArray())
				{
					TinyAssert(depVal.IsString());
					depVec.emplace_back(depVal.GetString());
				}
			}
			else if(itOneItem->name == JSON_NAME_PARAMS)
			{
				TinyAssert(itOneItem->value.IsArray());
				std::vector<ShaderMetaInfo::ParamInfo>& params = mapItem._metaInfo._params;
				for(const auto& param : itOneItem->value.GetArray())
				{
					TinyAssert(param.IsObject());
					TinyAssert(param.HasMember(JSON_NAME_PARAM_NAME));
					TinyAssert(param.HasMember(JSON_NAME_PARAM_SLOT));
					params.emplace_back(ShaderMetaInfo::ParamInfo{ param[JSON_NAME_PARAM_SLOT].GetInt(), param[JSON_NAME_PARAM_NAME].GetString() });
				}
			}
			else if(itOneItem->name == JSON_NAME_DECLEARATION)
			{
				TinyAssert(itOneItem->value.IsArray());
				std::vector<ShaderDeclear>& declears = mapItem._metaInfo._declears;
				for (const auto& declear : itOneItem->value.GetArray())
				{
					TinyAssert(declear.IsObject());
					TinyAssert(declear.HasMember(JSON_NAME_SHADER_NAME));
					TinyAssert(declear.HasMember(JSON_NAME_SHADER_TYPE));
					TinyAssert(declear.HasMember(JSON_NAME_SHADER_ENTRY));
					TinyAssert(declear.HasMember(JSON_NAME_SHADER_DEFINES));
					const Value& name = declear[JSON_NAME_SHADER_NAME];
					const Value& type = declear[JSON_NAME_SHADER_TYPE];
					const Value& entry = declear[JSON_NAME_SHADER_ENTRY];
					ShaderDeclear buffer{ name.GetString(), (type == "vs" ? ShaderType::VS : ShaderType::PS), entry.GetString() ,{}};
					const Value& defs = declear[JSON_NAME_SHADER_DEFINES];
					TinyAssert(defs.IsArray());
					for(const auto& def : defs.GetArray())
					{
						TinyAssert(def.IsString());
						buffer._defines.emplace_back(def.GetString());
					}
					declears.emplace_back(std::move(buffer));
				}
			}
		}
	}
	return true;
}

bool ShaderMetaMgr::writeDependenceCacheToFile()
{
	File file;
	file.open(_recordFileName.getAbsolutePath(), File::AccessMode::WRITE);
	if (!file.isOpened())
	{
		DebugString("Open dependence record file [%s] failed", _recordFileName.getAbsolutePath().c_str());
		return false;
	}

	rapidjson::Document doc(kObjectType);

	for (auto& item : _dependence)
	{
		const Path& filePath = item.first;
		const MetaInfoMapItem& depItem = item.second;
		if (!depItem._otherInfo._used)
			continue;

		Value oneItem(kObjectType);
		oneItem.AddMember(JSON_NAME_TIME_STAMP, depItem._dependdInfo._timeStamp,doc.GetAllocator());

		Value depArr(kArrayType);
		for(auto& path : depItem._dependdInfo._dependences)
		{
			const std::string& name = path.getRelativePath();
			depArr.PushBack(Value().SetString(name.c_str(), static_cast<SizeType>(name.length())).Move(), doc.GetAllocator());
		}
		oneItem.AddMember(JSON_NAME_DEPENDENCE, depArr, doc.GetAllocator());

		Value paramArr(kArrayType);
		for (auto& param : depItem._metaInfo._params)
		{
			Value oneParam(kObjectType);
			oneParam.AddMember(JSON_NAME_PARAM_NAME, Value().SetString(param._name.c_str(), static_cast<SizeType>(param._name.length())).Move(), doc.GetAllocator());
			oneParam.AddMember(JSON_NAME_PARAM_SLOT, param._index, doc.GetAllocator());
			paramArr.PushBack(oneParam, doc.GetAllocator());
		}
		oneItem.AddMember(JSON_NAME_PARAMS, paramArr, doc.GetAllocator());

		Value declearationArr(kArrayType);
		for(auto& declear : depItem._metaInfo._declears)
		{
			Value oneDeclear(kObjectType);
			oneDeclear.AddMember(JSON_NAME_SHADER_NAME, Value().SetString(declear._name.c_str(), static_cast<SizeType>(declear._name.length())).Move(), doc.GetAllocator());
			oneDeclear.AddMember(JSON_NAME_SHADER_TYPE, (declear._shaderType == ShaderType::VS ? StringRef("vs") : StringRef("ps")), doc.GetAllocator());
			oneDeclear.AddMember(JSON_NAME_SHADER_ENTRY, Value().SetString(declear._entry.c_str(), static_cast<SizeType>(declear._entry.length())).Move(), doc.GetAllocator());
			Value defines(kArrayType);
			for(auto& def : declear._defines)
			{
				defines.PushBack(Value().SetString(def.c_str(), static_cast<SizeType>(def.length())), doc.GetAllocator());
			}
			oneDeclear.AddMember(JSON_NAME_SHADER_DEFINES, defines,doc.GetAllocator());
			declearationArr.PushBack(oneDeclear, doc.GetAllocator());
		}
		oneItem.AddMember(JSON_NAME_DECLEARATION, declearationArr, doc.GetAllocator());

		const std::string& name = filePath.getRelativePath();
		doc.AddMember(Value().SetString(name.c_str(), static_cast<SizeType>(name.length())).Move(), oneItem, doc.GetAllocator());
	}

	rapidjson::StringBuffer buffer;
	PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	bool formatScceed = doc.Accept(writer);
	TinyAssert(formatScceed);
	file.write(buffer.GetString(), static_cast<int>(buffer.GetLength()));
	file.setEndOfFile();
	file.close();
	return true;
}

DependenceInfo ShaderMetaMgr::getDependent(const Path& file)
{
	MetaInfoMapItem* info = getMapItem(file);
	return waitDependenceInfo(info);
}

ShaderMetaInfo ShaderMetaMgr::getMetaInfo(const Path& file)
{
	MetaInfoMapItem* info = getMapItem(file);
	return waitShaderMetaInfo(info);
}

ShaderMetaMgr::MetaInfoMapItem* ShaderMetaMgr::getMapItem(const Path& file)
{
	MetaInfoMapItem* info = readMap(file);
	if(!info)
	{
		info = allocMap(file);
	}
	else
	{
		refreshInfo(file, info);
	}
	TinyAssert(info);
	if (!info->_otherInfo._used)
		info->_otherInfo._used = true;
	return info;
}

ShaderMetaMgr::MetaInfoMapItem* ShaderMetaMgr::readMap(const Path& file)
{
	ReadLock(_dependenceMutex);
	auto it = _dependence.find(file);
	if (it == _dependence.end())
		return nullptr;
	return &it->second;
}

ShaderMetaMgr::MetaInfoMapItem* ShaderMetaMgr::allocMap(const Path& file)
{
	WriteLock mapWriteLock(_dependenceMutex);
	MetaInfoMapItem& item = _dependence[file];
	if(item._otherInfo._status == OtherInfo::Inited)
	{
		WriteLock dataLock(item._otherInfo._mutex);
		item._otherInfo._status = OtherInfo::Working;
		mapWriteLock.unlock();
		bool result = readDependenceAndMetaInFile(file, &item);//todo handle read fail
		TinyAssert(result);
	}
	return &item;
}

void ShaderMetaMgr::refreshInfo(const Path& path, MetaInfoMapItem* info)
{
	TinyAssert(info);
	WriteLock lock(info->_otherInfo._mutex);
	if (info->_otherInfo._status != OtherInfo::Ready)
		return;

	if (info->_dependdInfo._timeStamp == getTimeStamp(path))
		return;

	info->_otherInfo._status = OtherInfo::Refreshing;
	bool result = readDependenceAndMetaInFile(path, info);//todo handle read fail
	TinyAssert(result);
}

DependenceInfo ShaderMetaMgr::waitDependenceInfo(MetaInfoMapItem* info)
{
	TinyAssert(info);
	ReadLock lock(info->_otherInfo._mutex);
	TinyAssert(info->_otherInfo._status == OtherInfo::Ready);
	return info->_dependdInfo;
}

ShaderMetaInfo ShaderMetaMgr::waitShaderMetaInfo(MetaInfoMapItem* info)
{
	TinyAssert(info);
	ReadLock lock(info->_otherInfo._mutex);
	TinyAssert(info->_otherInfo._status == OtherInfo::Ready);
	return info->_metaInfo;
}

bool ShaderMetaMgr::readDependenceAndMetaInFile(const Path& file, MetaInfoMapItem* info)
{
	TinyAssert(info && (info->_otherInfo._status == OtherInfo::Working || info->_otherInfo._status == OtherInfo::Refreshing));
	TinyAssert(info->_otherInfo._mutex.try_lock() == false);//The data lock must be already locked

	std::ifstream shaderFile(file.getAbsolutePath());
	if(!shaderFile.is_open())
	{
		return false;
	}

	info->_dependdInfo._timeStamp = getTimeStamp(file);
	info->_dependdInfo._dependences.clear();
	info->_metaInfo._declears.clear();
	info->_metaInfo._params.clear();

	std::string line;
	while(std::getline(shaderFile, line))
	{
		static const std::regex declear("DECLEAR_SHADER:([a-zA-Z_0-9]+)");
		std::smatch mr;
		if (std::regex_search(line, mr, declear))
		{
			ShaderDeclear dec;
			TinyAssert(mr.size() == 2);
			dec._name = mr[1].str();

			static const std::regex shaderType("SHADER_TYPE:(vs|ps)");
			if (std::regex_search(line, mr, shaderType))
			{
				TinyAssert(mr.size() == 2);
				dec._shaderType = (mr[1].str() == "vs" ? ShaderType::VS : ShaderType::PS);
			}
			else
				continue;

			static const std::regex entry("ENTRY:([a-zA-Z_0-9]+)");
			if (std::regex_search(line, mr, entry))
			{
				TinyAssert(mr.size() == 2);
				dec._entry = mr[1].str();
			}
			else
				continue;

			static const std::regex define("DEFINE:([a-zA-Z_0-9,]+)");
			if (std::regex_search(line, mr, define))
			{
				TinyAssert(mr.size() == 2);
				dec._defines = split(mr[1].str(), ",");
			}

			info->_metaInfo._declears.push_back(dec);
			continue;
		}

		static const std::regex param("PARAM:([a-zA-Z_0-9]+)");
		if (std::regex_search(line, mr, param))
		{
			ShaderMetaInfo::ParamInfo paramInfo;
			TinyAssert(mr.size() == 2);
			paramInfo._name = mr[1].str();

			static const std::regex regis(R"(register *\(c(\d+) *\))");
			if (std::regex_search(line, mr, regis))
			{
				TinyAssert(mr.size() == 2);
				paramInfo._index = atoi(mr[1].str().c_str());
			}
			info->_metaInfo._params.emplace_back(paramInfo);
			continue;
		}

		static const std::regex include("#include *\"([a-zA-Z_0-9.]+)\"");
		if (std::regex_search(line, mr, include))
		{
			TinyAssert(mr.size() == 2);
			Path path = file.getParentDirectory().getRelativePath() + mr[1].str();
			info->_dependdInfo._dependences.emplace_back(path);
		}
	}
	info->_otherInfo._status = OtherInfo::Ready;

	return true;
}
