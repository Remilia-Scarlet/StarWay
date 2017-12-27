#define _CRT_SECURE_NO_WARNINGS
#include "precomp.h"
#include "CommonFunc.h"
#include <fstream>

#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
#include <windows.h>
#endif

void DebugString(const char* fmt, ...)
{
	if (!fmt)
		return;
	std::string format = fmt;
	format += "\n";
	static std::unique_ptr<char[]> buffer(new char[10240]);
	va_list args;
	va_start(args, fmt);
	vsprintf(buffer.get(), format.c_str(), args);
	va_end(args);
#if TINY_PLATFORM_TARGET == TINY_PLATFORM_WINDOWS
	OutputDebugStringA(buffer.get());
#endif
	if (!NO_LOG_FILE)
	{
		std::ofstream logFile("log.txt", std::ios::out | std::ios::app);
		if (logFile)
		{
			logFile << buffer.get() << std::endl;
			logFile.close();
		}
	}
	printf(buffer.get());
}


std::string FormatString(const char* format, ...)
{
	va_list _ArgList;
	va_start(_ArgList, format);
	static std::unique_ptr<char[]> buffer(new char[10240]);
	vsprintf(buffer.get(), format, _ArgList);
	va_end(_ArgList);
	return buffer.get();
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
