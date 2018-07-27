#pragma once
#include <cstdint>
#include <vector>
#include <map>
#include <atomic>
#include <shared_mutex>
#include "ShaderMetaInfo.h"
#include <boost/serialization/vector.hpp>

struct DependenceInfo
{
	uint64_t _timeStamp = 0;
	std::vector<Path> _dependences;
	friend class boost::serialization::access;
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version)
	{
		ar & _timeStamp;
		ar & _dependences;
	}
};

class ShaderMetaMgr
{
	typedef std::shared_lock<std::shared_mutex> ReadLock;
	typedef std::unique_lock<std::shared_mutex> WriteLock;
public:
	ShaderMetaMgr();
	virtual ~ShaderMetaMgr();
public:
	void setRecordFile(const Path& file);
	// only called from main thread
	bool parseDependenceCache();

	// may be called from different threads
	DependenceInfo getDependent(const Path& file);

	// may be called from different thread
	ShaderMetaInfo getMetaInfo(const Path& file);

	// only called from main thread
	bool writeDependenceCacheToFile();

	void setIncludePath(const std::vector<Path>& paths);
protected:
	struct OtherInfo
	{
		enum Status
		{
			Inited,
			Working,
			Refreshing,
			Ready
		};
		Status _status = Inited;
		std::shared_mutex _mutex;
		std::atomic<bool> _used = false;
	};
	struct MetaInfoMapItem
	{
		DependenceInfo _dependdInfo;
		ShaderMetaInfo _metaInfo;
		std::shared_ptr<OtherInfo> _otherInfo{ new OtherInfo };
		friend class boost::serialization::access;
		template<class Archive>
		void save(Archive & ar, const unsigned int version) const
		{
			ar << _dependdInfo;
			ar << _metaInfo;
		}
		template<class Archive>
		void load(Archive & ar, const unsigned int version)
		{
			ar >> _dependdInfo;
			ar >> _metaInfo;
			_otherInfo->_status = OtherInfo::Ready;
		}
		BOOST_SERIALIZATION_SPLIT_MEMBER()
	};
	
	std::map<Path, MetaInfoMapItem > _dependence;
	std::shared_mutex _dependenceMutex;
	Path _recordFileName;
	std::vector<Path> _includePath;

	MetaInfoMapItem* getMapItem(const Path& file);
	MetaInfoMapItem * readMap(const Path& file);
	MetaInfoMapItem* allocMap(const Path& file);
	void refreshInfo(const Path& path, MetaInfoMapItem* info);
	DependenceInfo waitDependenceInfo(MetaInfoMapItem* info);
	ShaderMetaInfo waitShaderMetaInfo(MetaInfoMapItem* info);
	bool readDependenceAndMetaInFile(const Path& file, MetaInfoMapItem* info);
};

