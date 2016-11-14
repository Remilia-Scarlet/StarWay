#define _CRT_SECURE_NO_WARNINGS
#include "Ash/precomp.h"
#include "CommonFunc.h"
#include <fstream>

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
#include <windows.h>
#endif

void DebugString(std::string format, ...)
{
	format += "\n";
	static char buffer[1024];
	va_list args;
	va_start(args, format);
#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	vsprintf_s(buffer, format.c_str(), args);
#else
	vsprintf(buffer, format.c_str(), args);
#endif
	va_end(args);
#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	OutputDebugStringA(buffer);
#endif
#if !NO_LOG_FILE 
	std::ofstream logFile("log.txt", std::ios::out | std::ios::app);
	if (logFile)
	{
		logFile << buffer << std::endl;
		logFile.close();
	}
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
	if (separator.empty())
	{
		TinyAssert(false, "split must take a separator not empty");
		return ret;
	}

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

std::string& replaceSubstr(std::string& src, const std::string& searchKey, const std::string& targetStr)
{
	if (src.empty() || searchKey.empty())
		return src;

	size_t pos = 0;
	size_t oldPos = pos;
	while (1)
	{
		pos = src.find(searchKey, oldPos);
		if(pos == -1)
			break;
		src.replace(pos, searchKey.length(), targetStr);
		oldPos = pos + targetStr.length();
	}
	return src;
}
