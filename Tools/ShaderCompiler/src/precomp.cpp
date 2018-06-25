#include "precomp.h"
#include <Windows.h>
#include "Ash/FileSystem/Path_Win.h"

uint64_t getTimeStamp(const Path& file)
{
	WIN32_FIND_DATA desInfo;
	HANDLE hFind = FindFirstFile(file.getAbsolutePath().c_str(), &desInfo);
	FindClose(hFind);
	if (hFind == INVALID_HANDLE_VALUE)
		return 0;

	return uint64_t(desInfo.ftLastWriteTime.dwHighDateTime) << 32 | uint64_t(desInfo.ftLastWriteTime.dwLowDateTime);
}
