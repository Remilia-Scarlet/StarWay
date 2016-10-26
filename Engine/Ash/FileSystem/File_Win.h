#pragma once
#include "Path_Win.h"
#include <vector>

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS

#include <windows.h>

class File
{
public:
	enum class AccessMode
	{
		// open the file for read
		READ = 1,
		// open the file for wirte
		WRITE = 2,
		// open the file for both read and write
		READ_WRITE = READ | WRITE
	};
	enum class CreateMode
	{
		// If the file doesn't exist, create it. 
		ALWAYS_CREATE = 1,
		// Open a file only when it already exist.
		OPEN_EXIST = 2
	};
public:
	File();
	File(const Path& path, AccessMode accessMode = AccessMode::READ_WRITE, CreateMode createMode = CreateMode::ALWAYS_CREATE);
	//Can't copy. use std::move(file)
	File(const File& other) = delete;
	File(File&& other);
	~File();

	bool open();
	bool open(const Path& path);
	void close();

	std::vector<char> readAll();

	bool isValid() const;
	bool isOpened() const;
	bool setFilePath(const Path& path);
	const Path& getFilePath() const;
protected:
	Path _filePath;
	AccessMode _accessMode;
	CreateMode _createMode;
	HANDLE _fileHandle;
};

#endif //#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS