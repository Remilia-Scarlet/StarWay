#pragma once
#if !defined(FS_INCLUDE)
#error Don't include this file directly. Include "fs_include.h" instead.
#endif

#if defined(TINY_PLATFORM_WINDOWS)

#include "Path_Win.h"
#include <vector>
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
		OPEN_AWAYS = 1,
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
	bool open(const Path& path, AccessMode accessMode = AccessMode::READ_WRITE, CreateMode createMode = CreateMode::OPEN_AWAYS);

	// Close a file and release it's resource.
	void close();

	// Seek to a special position. Read and write will start from this position.
	void seek(int pos);

	// Get current position
	int pos();

	// Set current pos to file end. It can increase or decrease file size
	bool setEndOfFile();

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

#endif //#if defined(TINY_PLATFORM_WINDOWS)
