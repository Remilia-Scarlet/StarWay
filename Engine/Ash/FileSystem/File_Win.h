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
	//Can't copy. use std::move(file)
	File(const File& other) = delete;
	File(File&& other);
	~File();

	// Before you can use a File, you must open it. And remember to close() it when you don't need it. Destructor will also call close() automaticlly.
	bool open(const Path& path, AccessMode accessMode = AccessMode::READ_WRITE, CreateMode createMode = CreateMode::ALWAYS_CREATE);

	// Close a file and release it's resource.
	void close();

	// Seek to a special position. Read and write will start from this position.
	void seek(int pos);

	// Get current position
	int pos();

	// Get the directory of this file
	Path getDirectory();

	std::vector<char> readAll();

	// write data
	bool write(const void* data, int len);

	// Can't copy, close this file and call setFilePath(newPath) if you need to open a new file
	File& operator=(const File& other) = delete;
	File& operator=(File&& other) = delete;

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