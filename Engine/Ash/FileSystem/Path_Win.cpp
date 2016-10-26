#include "Ash/precomp.h"
#include "Path_Win.h"

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS

#include <windows.h>
#include <stack>

void Path::_getSubFile(std::list<Path>& list) const
{
	if (!isDirectory())
		return;

	std::string path = getAbsolutePath() + "*.*";
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
	getAbsolutePath();
	if (_absolutePath == "")
		return false;
	DWORD att = GetFileAttributes(_absolutePath.c_str());
	if (att == FILE_INVALID_FILE_ID)
		return false;
	return (att & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool Path::isFile() const
{
	getAbsolutePath();
	if (_absolutePath == "")
		return false;

	DWORD att = GetFileAttributes(_absolutePath.c_str());
	if (att == FILE_INVALID_FILE_ID)
		return false;
	return (att & FILE_ATTRIBUTE_DIRECTORY) == 0;
}

std::list<Path> Path::getSubFile() const
{
	std::list<Path> ret;
	_getSubFile(ret);
	return ret;
}

const std::string& Path::getOriginPath() const
{
	return _path;
}

const std::string& Path::getAbsolutePath() const
{	
	if (_absolutePath.length() > 0)
		return _absolutePath;

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
	return _absolutePath;
}

const std::string& Path::getRelativePath() const
{
	char* path = new char[1024];
	std::string myPath = _fullpath(path, ".", 1024);
	

	delete[] path;

}

#endif //#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS