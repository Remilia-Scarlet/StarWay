#if defined(TINY_PLATFORM_WINDOWS)
#include "../fs_include.h"
#include <algorithm>
#include <windows.h>
#include <stack>
#include "Ash/TinyAssert.h"
#include <boost/filesystem.hpp>

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

bool Path::createDirectory()
{
	if (getAbsolutePath() == "")
		return false;

	BOOL ret = CreateDirectory(getAbsolutePath().c_str(), NULL);
	if (ret || (GetLastError() == ERROR_ALREADY_EXISTS))
	{
		if (isFile())
			return false;
		return true;
	}

	if (!getParentDirectory().createDirectory())
		return false;
	
	ret = CreateDirectory(getAbsolutePath().c_str(), NULL);
	if (ret)
	{
		_isDirectory = true;
		_isFile = false;
		_isDirectoryFileDirty = false;
		return true;
	}
	return false;
}

std::string Path::getFileName() const
{
	size_t pos = _path.find_last_of('\\');
	if (pos == _path.length())
		pos = _path.find_last_of('\\', pos - 1);
	if(pos != -1 && pos != 0 && pos != _path.length() - 1)
	{
		return _path.substr(pos + 1);
	}
	return "";
}

std::string Path::getFileNameWithoutExt() const
{
	std::string fileName = getFileName();
	if (fileName.empty())
		return "";

	size_t pos = fileName.find_last_of('.');
	if(pos != -1 && pos != fileName.length() - 1)
	{
		return fileName.substr(0, pos);
	}
	return std::string();
}

std::string Path::getFileExtension() const
{
	std::string fileName = getFileName();
	if (fileName.empty())
		return "";

	size_t pos = fileName.find_last_of('.');
	if (pos != -1 && pos != fileName.length() - 1)
	{
		return fileName.substr(pos + 1);
	}
	return std::string();
}

bool Path::deleteMe()
{
	boost::filesystem::remove_all(boost::filesystem::path(getAbsolutePath()));
	return true;
}

bool Path::operator<(const Path& another) const
{
	return _path < another._path;
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
	std::transform(_path.begin(), _path.end(), _path.begin(), [](char ch)
	{
		if (ch == '/')
			ch = '\\';
		return ch;
	});
}

Path::Path(const char* path)
	:_path(path)
	, _absolutePathDirty(true)
	, _relativePathDirty(true)
	, _isDirectoryFileDirty(true)
{
	std::transform(_path.begin(), _path.end(), _path.begin(), [](char ch)
	{
		if (ch == '/')
			ch = '\\';
		return ch;
	});
}

Path::Path(const std::string& path,
		   const std::string& absolutePath,
		   bool absolutePathDirty,
		   const std::string& relativePath,
		   bool relativePathDirty,
		   bool isDirectory,
		   bool isFile,
		   bool isDirectoryFileDirty)
	: _path(path)
	, _absolutePath(absolutePath)
	, _absolutePathDirty(absolutePathDirty)
	, _relativePath(relativePath)
	, _relativePathDirty(relativePathDirty)
	, _isDirectory(isDirectory)
	, _isFile(isFile)
	, _isDirectoryFileDirty(isDirectoryFileDirty)
{
}

bool Path::isDirectory() const
{
	if (!_isDirectoryFileDirty)
		return _isDirectory;

	_getIsDirectory(GetFileAttributes(getAbsolutePath().c_str()));
	return _isDirectory;
}

bool Path::isFile() const
{
	if (!_isDirectoryFileDirty)
		return _isFile;

	_getIsDirectory(GetFileAttributes(getAbsolutePath().c_str()));
	return _isFile;
}

bool Path::isValid() const
{
	return isDirectory() || isFile();
}

std::list<Path> Path::getFileList(const std::string& filter) const
{
	std::stack<Path> stack;
	std::list<Path> list;
	stack.push(*this);
	while (!stack.empty())
	{
		Path current = stack.top();
		stack.pop();
		if (!current.isDirectory())
			continue;

		std::string path = current.getAbsolutePath() + filter;
		WIN32_FIND_DATA findFileData;
		HANDLE hFind = ::FindFirstFile(path.c_str(), &findFileData);
		if (INVALID_HANDLE_VALUE == hFind)
			continue;
		do
		{
			if (strcmp(findFileData.cFileName, ".") != 0 && strcmp(findFileData.cFileName, "..") != 0)
			{
				std::string currentPath = current.getAbsolutePath() + findFileData.cFileName;
				if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					stack.push(
						Path(
							currentPath,
							currentPath,
							false,
							"",
							true,
							true,
							false,
							false
							)
						);
				}
				else
				{
					list.push_back(
						Path(
							currentPath,
							currentPath,
							false,
							"",
							true,
							false,
							true,
							false
							)
						);
				}
			}
		} while (::FindNextFile(hFind, &findFileData));
		FindClose(hFind);
	}
	return list;
}

Path Path::getRootPath()
{
	return getExePath().getParentDirectory();
}

Path Path::getExePath()
{
	std::string ret;
	const int arrSize = 500;
	char* path = new char[arrSize];
	GetModuleFileNameA(NULL, path, arrSize);
	ret = path;
	_fullpath(path, ret.c_str(), arrSize);
	ret = path;
	delete[] path;
	return Path(ret, ret, false, "", true, true, false, false);
}

std::list<Path> Path::getFileList() const
{
	return getFileList("*.*");
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
		_absolutePath = getRootPath().getAbsolutePath() + _path.substr(offSet);
	}
	char* path = new char[1024];
	_fullpath(path, _absolutePath.c_str(), 1024);
	_absolutePath = path;
	delete[] path;
	return _absolutePath;
}

const std::string& Path::getRelativePath() const
{
	if (!_relativePathDirty)
		return _relativePath;
	_relativePathDirty = false;

	if (getAbsolutePath().empty())
	{
		_relativePath = "";
		return _relativePath;
	}

	char* path = new char[1024];
	std::string myPath = _fullpath(path, ".", 1024);
	delete[] path;

	std::vector<std::string> path1 = Ash::split(myPath,"\\");
	std::vector<std::string> path2 = Ash::split(getAbsolutePath(),"\\");
	int i = 0;

	while (i < (int)min(path1.size(), path2.size()) && Ash::toLower(path1[i]) == Ash::toLower(path2[i]))
		++i;

	int j = i;
	for (; j < (int)path1.size(); ++j)
		_relativePath += "..\\";

	for (; i < (int)path2.size(); ++i)
	{
		if(!_relativePath.empty() && _relativePath.back() != '\\')
			_relativePath += "\\";
		_relativePath += path2[i];
	}
	if(getAbsolutePath().back() == '\\'
		&& (!_relativePath.empty() && _relativePath.back() != '\\'))
	{
		_relativePath += '\\';
	}

	if (_relativePath.length() == 0)
		_relativePath = ".\\";
	return _relativePath;
}

Path Path::getParentDirectory() const
{
	size_t pos = getAbsolutePath().find_last_of('\\');
	if (pos + 1u == getAbsolutePath().length())
		pos = getAbsolutePath().find_last_of('\\', pos - 1);
	if (pos != -1 && pos != 0)
	{
		std::string path = _absolutePath.substr(0, pos + 1);
		return Path(
			path,
			path,
			false,
			"",
			true,
			true,
			false,
			_isDirectoryFileDirty || (!_isFile && !_isDirectory)
			);
	}
	return Path();
}

#endif //#if defined(TINY_PLATFORM_WINDOWS)
