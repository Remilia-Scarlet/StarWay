#include "TinyEngine\precomp.h"
#include "Path.h"

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	#include <windows.h>
#else
	#error not support
#endif

void Path::_getSubPath(std::list<Path>& list) const
{
	if (!isDirectory())
		return;

	std::string path = getResolvedPath() + "\\*.*";
#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	WIN32_FIND_DATA findFileData;
	HANDLE hFind = ::FindFirstFile(path.c_str(), &findFileData);
	if (INVALID_HANDLE_VALUE == hFind)
		return;
	do
	{
		if (strcmp(findFileData.cFileName,".") != 0 && strcmp(findFileData.cFileName, "..") != 0)
		{
			if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				Path(getResolvedPath() + "\\" + findFileData.cFileName)._getSubPath(list);
			}
			else
			{
				Path p;
				p._path = FormatString("%s\\%s", getResolvedPath().c_str(), findFileData.cFileName);
				p._resolvedPath = p._path;
				list.push_back(p);
			}
		}
	} while (::FindNextFile(hFind, &findFileData));
#else
	#error not support
#endif
}

Path::Path()
{

}

Path::Path(const std::string& path)
	:_path(path)
{

}

Path::Path(const char* path)
	:_path(path)
{

}

bool Path::isDirectory() const
{
	getResolvedPath();
	if (_resolvedPath == "")
		return false;

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	DWORD att = GetFileAttributes(_resolvedPath.c_str());
	if (att == FILE_INVALID_FILE_ID)
		return false;
	return (att & FILE_ATTRIBUTE_DIRECTORY) != 0;
#else
	#error not support
#endif
}

bool Path::isFile() const
{
	getResolvedPath();
	if (_resolvedPath == "")
		return false;

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	return false;
#else
	#error not support
#endif
}

std::list<Path> Path::getSubPath() const
{
	std::list<Path> ret;
	_getSubPath(ret);
	return ret;
}

const std::string& Path::getResolvedPath() const
{	
	if (_resolvedPath.length() > 0)
		return _resolvedPath;

	_resolvedPath = _path;
#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	if (_path.find(GAME_PATH) == 0)
	{
		size_t offSet = strlen(GAME_PATH);
		if (_path[offSet] == '\\' || _path[offSet] == '/')
			++offSet;
		_resolvedPath = _path.substr(offSet);
	}
	char* path = new char[1024];
	_fullpath(path, _resolvedPath.c_str(), 1024);
	_resolvedPath = path;
	delete[] path;
	DWORD att = GetFileAttributes(_resolvedPath.c_str());
	if (att == (DWORD)FILE_INVALID_FILE_ID)
		_resolvedPath = "";
	if (_resolvedPath.length() > 0 && (_resolvedPath[_resolvedPath.length() - 1] == '\\' || _resolvedPath[_resolvedPath.length() - 1] == '/'))
		_resolvedPath = _resolvedPath.substr(0, _resolvedPath.length() - 1);
#else
	#error not support
#endif;
	return _resolvedPath;
}
