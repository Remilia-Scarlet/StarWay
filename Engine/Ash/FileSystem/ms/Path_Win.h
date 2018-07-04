#pragma once

#if !defined(FS_INCLUDE)
#error Don't include this file directly. Include "fs_include.h" instead.
#endif

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS

#include <list>
#include <windows.h>

static const char* GAME_PATH = "game:";

class File;
class Path
{
public:
	Path();

	// Init with given path
	Path(const std::string& path);
	Path(const char* path);

	// Check if path is directoy
	bool isDirectory() const;

	// Check if path is file
	bool isFile() const;

	// Check if path is directory or file
	bool isValid() const;

	// List all files under this directory and its sub directories. If this path is a file, it returns a empty list.
	std::list<Path> getFileList() const;

	// List all files under this directory and its sub directories with special filter.
	// For example getFileList("*.txt") will return a list with all txt file under this folder and sub folders.
	// If this path is a file, it returns a empty list.
	std::list<Path> getFileList(const std::string& filter) const;

	// Return the exe file path
	Path getRootPath() const;

	// Return the original path which you set to Path
	const std::string& getOriginPath() const;

	// Return the absolute path
	const std::string& getAbsolutePath() const;

	// Return relative path to current work path
	const std::string& getRelativePath() const;

	// Get the folder which contains this folder or file
	Path getParentDirectory() const;

	// Create a folder at this path
	bool createDirectory();

	// Get the file name with extension. If the path is not file, it returns empty string
	std::string getFileName() const;

	// Get the file name without extension. If the path is not file, it returns empty string
	std::string getFileNameWithoutExt() const;

	// Get the extension of file name
	std::string getFileExtension() const;

	// For container std::map
	bool operator<(const Path& another) const;
protected:
	// Init a Path with given status. Be careful you must provide the exactly right status. DON'T use this constructor unless you understand what you are doing.
	Path(
		const std::string& path,

		const std::string& absolutePath,
		bool absolutePathDirty,

		const std::string& relativePath,
		bool relativePathDirty,

		bool isDirectory,
		bool isFile,
		bool isDirectoryFileDirty
	);

	std::string _path;

	mutable std::string _absolutePath;
	mutable bool _absolutePathDirty;

	mutable std::string _relativePath;
	mutable bool _relativePathDirty;

	mutable bool _isDirectory;
	mutable bool _isFile;
	mutable bool _isDirectoryFileDirty;

	void _getIsDirectory(DWORD att) const;

};

#endif