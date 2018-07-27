#include "precomp.h"
#include "ShaderMetaMgr.h"
#include <TinyEngine/Engine/EngineDefs.h>
#include "Ash/FileSystem/fs_include.h"
#include <fstream>
#include <sstream>
#include "Ash/CommonStateMachine/StateMachine.h"
#include <regex>
#include "Ash/Container/ArrayStream.h"
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/serialization/map.hpp>


ShaderMetaMgr::ShaderMetaMgr()
{
}


ShaderMetaMgr::~ShaderMetaMgr()
{
}

void ShaderMetaMgr::setRecordFile(const Path& file)
{
	_recordFileName = file;
}

bool ShaderMetaMgr::parseDependenceCache()
{
	File file;
	file.open(_recordFileName.getAbsolutePath(), File::AccessMode::READ, File::CreateMode::OPEN_EXIST);
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
	ia >> _dependence;

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

	for(auto it = _dependence.begin(); it != _dependence.end() ;)
	{
		if (it->second._otherInfo->_used == false)
			it = _dependence.erase(it);
		else
			++it;
	}

	std::stringstream ss;
	boost::archive::binary_oarchive oa(ss);
	oa << _dependence;

	std::string str = ss.str();

	file.write(str.c_str(), static_cast<int>(str.size()));
	file.setEndOfFile();
	file.close();
	return true;
}

void ShaderMetaMgr::setIncludePath(const std::vector<Path>& paths)
{
	_includePath = paths;
}

DependenceInfo ShaderMetaMgr::getDependent(const Path& file)
{
	TinyAssert(file.isFile());
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
	if (!info->_otherInfo->_used)
		info->_otherInfo->_used = true;
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
	WriteLock dataLock(item._otherInfo->_mutex);
	if(item._otherInfo->_status == OtherInfo::Inited)
	{
		item._otherInfo->_status = OtherInfo::Working;
		mapWriteLock.unlock();
		bool result = readDependenceAndMetaInFile(file, &item);//todo handle read fail
		TinyAssert(result);
	}
	return &item;
}

void ShaderMetaMgr::refreshInfo(const Path& path, MetaInfoMapItem* info)
{
	TinyAssert(info);
	WriteLock lock(info->_otherInfo->_mutex);
	if (info->_otherInfo->_status != OtherInfo::Ready)
		return;

	if (info->_dependdInfo._timeStamp == getTimeStamp(path))
		return;

	info->_otherInfo->_status = OtherInfo::Refreshing;
	bool result = readDependenceAndMetaInFile(path, info);//todo handle read fail
	TinyAssert(result);
}

DependenceInfo ShaderMetaMgr::waitDependenceInfo(MetaInfoMapItem* info)
{
	TinyAssert(info);
	ReadLock lock(info->_otherInfo->_mutex);
	TinyAssert(info->_otherInfo->_status == OtherInfo::Ready);
	return info->_dependdInfo;
}

ShaderMetaInfo ShaderMetaMgr::waitShaderMetaInfo(MetaInfoMapItem* info)
{
	TinyAssert(info);
	ReadLock lock(info->_otherInfo->_mutex);
	TinyAssert(info->_otherInfo->_status == OtherInfo::Ready);
	return info->_metaInfo;
}

bool ShaderMetaMgr::readDependenceAndMetaInFile(const Path& file, MetaInfoMapItem* info)
{
	TinyAssert(info && (info->_otherInfo->_status == OtherInfo::Working || info->_otherInfo->_status == OtherInfo::Refreshing));
	TinyAssert(info->_otherInfo->_mutex.try_lock() == false);//The data lock must be already locked

	std::ifstream shaderFile(file.getAbsolutePath());
	if(!shaderFile.is_open())
	{
		return false;
	}

	info->_dependdInfo._timeStamp = getTimeStamp(file);
	info->_dependdInfo._dependences.clear();
	info->_metaInfo._declears.clear();

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

		static const std::regex include("#include *\"([a-zA-Z_0-9.]+)\"");
		if (std::regex_search(line, mr, include))
		{
			TinyAssert(mr.size() == 2);
			Path path = file.getParentDirectory().getRelativePath() + mr[1].str();
			if(!path.isFile())
			{
				for(auto& includePath : _includePath)
				{
					Path pathTmp = includePath.getRelativePath() + mr[1].str();
					if(pathTmp.isFile())
					{
						path = pathTmp;
						break;
					}
				}
			}
			info->_dependdInfo._dependences.emplace_back(path);
		}
	}
	info->_otherInfo->_status = OtherInfo::Ready;

	return true;
}
