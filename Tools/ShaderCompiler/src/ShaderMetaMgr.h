#pragma once
#include <cstdint>
#include <vector>
#include <map>
#include <atomic>
#include <shared_mutex>
#include "ShaderMetaInfo.h"

struct DependenceInfo
{
	uint64_t _timeStamp = 0;
	std::vector<Path> _dependences;
};

class ShaderMetaMgr
{
	typedef std::shared_lock<std::shared_mutex> ReadLock;
	typedef std::unique_lock<std::shared_mutex> WriteLock;
public:
	ShaderMetaMgr();
	virtual ~ShaderMetaMgr();
public:
	// only called from main thread
	bool parseDependenceCache(const Path& recordFileName);

	// may be called from different threads
	DependenceInfo getDependent(const Path& file);

	// may be called from different thread
	ShaderMetaInfo getMetaInfo(const Path& file);

	// only called from main thread
	bool writeDependenceCacheToFile();
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
		std::atomic<Status> _status = Inited;
		std::shared_mutex _mutex;
		std::atomic<bool> _used = false;
	};
	struct MetaInfoMapItem
	{
		DependenceInfo _dependdInfo;
		ShaderMetaInfo _metaInfo;
		OtherInfo _otherInfo;
	};
	
	std::map<Path, MetaInfoMapItem > _dependence;
	std::shared_mutex _dependenceMutex;
	Path _recordFileName;

	MetaInfoMapItem* getMapItem(const Path& file);
	MetaInfoMapItem * readMap(const Path& file);
	MetaInfoMapItem* allocMap(const Path& file);
	void refreshInfo(const Path& path, MetaInfoMapItem* info);
	DependenceInfo waitDependenceInfo(MetaInfoMapItem* info);
	ShaderMetaInfo waitShaderMetaInfo(MetaInfoMapItem* info);
	bool readDependenceAndMetaInFile(const Path& file, MetaInfoMapItem* info);
};

