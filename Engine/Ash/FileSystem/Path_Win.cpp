#include "Ash/precomp.h"
#include "Path_Win.h"

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS

#include <windows.h>
#include <stack>
#include "Ash/CommonFunc.h"

void Path::_getSubFile(std::list<Path>& list, const std::string& filter) const
{
	if (!isDirectory())
		return;

	std::string path = getAbsolutePath() + filter;
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
				Path(getAbsolutePath() + findFileData.cFileName)._getSubFile(list);
			}
			else
			{
				Path p;
				p._path = getAbsolutePath() + findFileData.cFileName;
				p._absolutePath = p._path;
				list.push_back(p);
			}
		}
	} while (::FindNextFile(hFind, &findFileData));
}

void Path::_getIsDirectory(DWORD att) const
{
	if (att == (DWORD)FILE_INVALID_FILE_ID)
		_isDirectory = _isFile = false;
	else
	{
		_isDirectory =((att & FILE_ATTRIBUTE_DIRECTORY) != 0);
		_isFile = ((att & FILE_ATTRIBUTE_DIRECTORY) == 0);
	}
	_isDirectoryFileDirty = false;
}

Path::Path()
	: _absolutePathDirty(true)
	, _relativePathDirty(true)
	, _isDirectoryFileDirty(true)
{

}

Path::Path(const std::string& path)
	:_path(path)
	, _absolutePathDirty(true)
	, _relativePathDirty(true)
	, _isDirectoryFileDirty(true)
{

}

Path::Path(const char* path)
	:_path(path)
	, _absolutePathDirty(true)
	, _relativePathDirty(true)
	, _isDirectoryFileDirty(true)
{

}

bool Path::isDirectory() const
{
	if (!_isDirectoryFileDirty)
		return _isDirectory;

	getRelativePath();
	if (_isDirectoryFileDirty)
		_getIsDirectory(GetFileAttributes(getRelativePath().c_str()));
	return _isDirectory;
}

bool Path::isFile() const
{
	if (!_isDirectoryFileDirty)
		return _isFile;

	getRelativePath();
	if (_isDirectoryFileDirty)
		_getIsDirectory(GetFileAttributes(getRelativePath().c_str()));
	return _isFile;
}

std::list<Path> Path::getSubFile(const std::string& filter) const
{
	std::list<Path> ret;
	_getSubFile(ret, filter);
	return ret;
}

std::list<Path> Path::getSubFile() const
{
	return getSubFile("*.*");
}

const std::string& Path::getOriginPath() const
{
	return _path;
}

const std::string& Path::getAbsolutePath() const
{	
	if (!_absolutePathDirty)
		return _absolutePath;
	_absolutePathDirty = false;

	_absolutePath = _path;
	if (_path.find(GAME_PATH) == 0)
	{
		size_t offSet = strlen(GAME_PATH);
		if (_path[offSet] == '\\' || _path[offSet] == '/')
			++offSet;
		_absolutePath = _path.substr(offSet);
	}
	char* path = new char[1024];
	_fullpath(path, _absolutePath.c_str(), 1024);
	_absolutePath = path;
	delete[] path;
	DWORD att = GetFileAttributes(_absolutePath.c_str());
	if (att == (DWORD)FILE_INVALID_FILE_ID)
		_absolutePath = "";
	else if ((att & FILE_ATTRIBUTE_DIRECTORY) != 0)
		_absolutePath += "\\";
	if(_isDirectoryFileDirty)
		_getIsDirectory(att);
	return _absolutePath;
}

const std::string& Path::getRelativePath() const
{
	if (!_relativePathDirty)
		return _relativePath;
	_relativePathDirty = false;

	if (getAbsolutePath().empty())
		return _relativePath;

	char* path = new char[1024];
	std::string myPath = _fullpath(path, ".", 1024);
	delete[] path;

	std::vector<std::string> path1 = split(myPath,"\\");
	std::vector<std::string> path2 = split(getAbsolutePath(),"\\");
	int i = 0;

	while (i < min(path1.size(), path2.size()) && path1[i] == path2[i])
		++i;

	int j = i;
	for (; j < path1.size(); ++j)
		_relativePath += "..\\";

	for (; i < path2.size(); ++i)
	{
		_relativePath += path2[i];
		_relativePath += "\\";
	}
	if (_relativePath.length() == 0)
		_relativePath = ".\\";
	if (isFile())
		_relativePath = _relativePath.substr(0, _relativePath.length() - 1);
	return _relativePath;
}

#endif //#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS