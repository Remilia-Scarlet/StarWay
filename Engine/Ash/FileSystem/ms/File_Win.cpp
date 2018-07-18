#include "Ash/precomp.h"

#if defined(TINY_PLATFORM_WINDOWS)
#include "../fs_include.h"
#include "Ash/TinyAssert.h"
#include <windows.h>

File::File() 
	: _accessMode(AccessMode::READ_WRITE)
	, _createMode(CreateMode::OPEN_AWAYS)
	, _fileHandle(INVALID_HANDLE_VALUE)
{
}

File::File(File&& other)
	: _filePath(std::move(other._filePath))
	, _accessMode(other._accessMode)
	, _createMode(other._createMode)
	, _fileHandle(other._fileHandle)
{
	other._fileHandle = INVALID_HANDLE_VALUE;
}

File::~File()
{
	close();
}

int AccessModeToWinAccessMode(File::AccessMode accessMode)
{
	int ret = 0;
	if ((int)accessMode & (int)File::AccessMode::READ)
		ret |= GENERIC_READ;
	if ((int)accessMode & (int)File::AccessMode::WRITE)
		ret |= GENERIC_WRITE;
	return ret;
}

int CreateModeToWinCreateMode(File::CreateMode createMode)
{
	switch (createMode)
	{
	case File::CreateMode::OPEN_AWAYS:
		return OPEN_ALWAYS;
	case File::CreateMode::OPEN_EXIST:
		return OPEN_EXISTING;
	}
	return CREATE_ALWAYS;
}

bool File::open(const Path& path, AccessMode accessMode /*= AccessMode::READ_WRITE*/, CreateMode createMode /*= CreateMode::ALWAYS_CREATE*/)
{
	if (isOpened())
	{
		TinyAssert(false, "you can't open a file twice");
		return false;
	}
	_filePath = path;
	_accessMode = accessMode;
	_createMode = createMode;

	Path folder = Path(getDirectory());
	if (!folder.isDirectory() && !folder.createDirectory())
		return false;
	
	_fileHandle = CreateFile(_filePath.getAbsolutePath().c_str(), AccessModeToWinAccessMode(_accessMode), 0, NULL, CreateModeToWinCreateMode(_createMode), 0, NULL);
	return isOpened();
}

void File::close()
{
	if(isOpened())
		CloseHandle(_fileHandle);
	_fileHandle = INVALID_HANDLE_VALUE;
}


void File::seek(int pos)
{
	if (!isOpened())
	{
		TinyAssert(false, "You must open file first");
		return;
	}
	SetFilePointer(_fileHandle, pos, NULL, FILE_BEGIN);
}

int File::pos()
{
	if (!isOpened())
	{
		TinyAssert(false, "You must open file first");
		return 0;
	}
	return SetFilePointer(_fileHandle, 0, NULL, FILE_CURRENT);
}

bool File::setEndOfFile()
{
	if (!isOpened())
	{
		TinyAssert(false, "You must open file first");
		return false;
	}
	return SetEndOfFile(_fileHandle) != 0;
}

Path File::getDirectory()
{
	return _filePath.getParentDirectory();
}

std::vector<char> File::readAll()
{
	std::vector<char> data;
	if (!isOpened())
	{
		TinyAssert(false, "You must open file first");
		return data;
	}
	seek(0);
	DWORD size = GetFileSize(_fileHandle, NULL);
	data.resize((size_t)size);
	DWORD readByte = 0;
	BOOL ret = ReadFile(_fileHandle, data.data(), size, &readByte, NULL);
	if (ret)
		return data;
	return std::vector<char>();
}

bool File::write(const void* data, int len)
{
	if (!isOpened())
	{
		TinyAssert(false, "you need to open first");
		return false;
	}
	int filePos = pos();
	LockFile(_fileHandle, filePos, 0, len, 0);
	DWORD writenBytes = 0;
	WriteFile(_fileHandle, data, len, &writenBytes, NULL);
	UnlockFile(_fileHandle, filePos, 0, len, 0);
	return false;
}

bool File::isValid() const
{
	return _filePath.isFile();
}

bool File::isOpened() const
{
	return _fileHandle != INVALID_HANDLE_VALUE;
}

bool File::setFilePath(const Path& path)
{
	if (isOpened())
	{
		TinyAssert(false, "You can't change file path when file is opened");
		return false;
	}
	_filePath = path;
	return _filePath.isFile();
}

const Path& File::getFilePath() const
{
	return _filePath;
}

#endif // #if defined(TINY_PLATFORM_WINDOWS)
