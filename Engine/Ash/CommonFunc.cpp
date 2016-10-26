#include "Ash/precomp.h"
#include "CommonFunc.h"
#define _CRT_SECURE_NO_WARNINGS

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
#include <windows.h>
#endif

void DebugString(std::string format, ...)
{
	format += "\n";
	static char buffer[1024];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format.c_str(), args);
	va_end(args);
#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	OutputDebugStringA(buffer);
#endif
	printf(buffer);
}


std::string FormatString(const char* format, ...)
{
	va_list _ArgList;
	va_start(_ArgList, format);
	char buffer[1024];
	vsprintf(buffer, format, _ArgList);
	va_end(_ArgList);
	return buffer;
}

std::vector<std::string> split(const std::string& src, const std::string& separator)
{
	std::vector<std::string> ret;
	size_t pos = 0;
	size_t oldPos = pos;

	bool breakLoop = false;
	while (!breakLoop)
	{
		breakLoop = (pos = src.find(separator, oldPos)) == -1;
		if (pos == -1)
			pos = src.length();
		if(pos < oldPos)
			break;
		size_t cutSize = pos - oldPos;
		if (cutSize != 0)
			ret.push_back(src.substr(oldPos, cutSize ));
		oldPos = pos + separator.length();
	}
	return ret;
}
