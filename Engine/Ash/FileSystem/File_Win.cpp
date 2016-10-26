#include "Ash/precomp.h"
#include "File_Win.h"

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS

File::File() 
	: _accessMode(AccessMode::READ_WRITE)
	, _createMode(CreateMode::ALWAYS_CREATE)
	, _fileHandle(INVALID_HANDLE_VALUE)
{
}

File::File(const Path& path, AccessMode accessMode /* = AccessMode::READ_WRITE*/, CreateMode createMode /* = CreateMode::ALWAYS_CREATE*/)
	: _filePath(path)
	, _accessMode(accessMode)
	, _createMode(createMode)
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
	if ((int)accessMode | (int)File::AccessMode::READ)
		ret |= GENERIC_READ;
	if ((int)accessMode | (int)File::AccessMode::WRITE)
		ret |= GENERIC_WRITE;
	return ret;
}

int CreateModeToWinCreateMode(File::CreateMode createMode)
{
	switch (createMode)
	{
	case File::CreateMode::ALWAYS_CREATE:
		return CREATE_ALWAYS;
	case File::CreateMode::OPEN_EXIST:
		return OPEN_EXISTING;
	}
	return CREATE_ALWAYS;
}
bool File::open()
{
	if (isOpened())
		return true;
	_fileHandle = CreateFile(_filePath.getAbsolutePath().c_str(), AccessModeToWinAccessMode(_accessMode), 0, NULL, CreateModeToWinCreateMode(_createMode), 0, NULL);
	return isOpened();
}

bool File::open(const Path & path)
{
	setFilePath(path);
	return open();
}

void File::close()
{
	if(isOpened())
		CloseHandle(_fileHandle);
	_fileHandle = INVALID_HANDLE_VALUE;
}


std::vector<char> File::readAll()
{
	std::vector<char> data;
	if (!isOpened())
	{
		TinyAssert(false, "You must open file first");
		return data;
	}
	
	DWORD size = GetFileSize(_fileHandle, NULL);
	data.resize((size_t)size);
	DWORD readByte = 0;
	BOOL ret = ReadFile(_fileHandle, data.data(), size, &readByte, NULL);
	if (ret)
		return data;
	return std::vector<char>();
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
	_filePath = path;
	return _filePath.isFile();
}

const Path& File::getFilePath() const
{
	return _filePath;
}

#endif // #if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
