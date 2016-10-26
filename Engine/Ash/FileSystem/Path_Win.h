#pragma once
#include <list>

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS

#include <windows.h>

static const char* GAME_PATH = "game:";

class File;
class Path
{
public:
	Path();
	Path(const std::string& path);
	Path(const char* path);

	bool isDirectory() const;
	bool isFile() const;

	std::list<Path> getSubFile() const;
	const std::string& getOriginPath() const;
	const std::string& getAbsolutePath() const;
	const std::string& getRelativePath() const;
protected:
	std::string _path;

	mutable std::string _absolutePath;
	mutable bool _absolutePathDirty;

	mutable std::string _relativePath;
	mutable bool _relativePathDirty;

	mutable bool _isDirectory;
	mutable bool _isFile;
	mutable bool _isDirectoryFileDirty;

	void _getSubFile(std::list<Path>& list) const;
	void _getIsDirectory(DWORD att) const;
};

#endif