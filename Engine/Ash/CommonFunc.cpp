#include "precomp.h"
#include "CommonFunc.h"
#include <fstream>
#include <memory>
#include "TinyAssert.h"

#if defined(TINY_PLATFORM_WINDOWS)
#include <Windows.h>
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
#if defined(TINY_PLATFORM_WINDOWS)
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

std::wstring charToWchar(const char * c)
{
#if defined(TINY_PLATFORM_WINDOWS)
	std::wstring str;
	int len = MultiByteToWideChar(CP_ACP, 0, c, (int)strlen(c), NULL, 0);
	wchar_t*    m_wchar = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, c, (int)strlen(c), m_wchar, len);
	m_wchar[len] = '\0';
	str = m_wchar;
	delete m_wchar;
	return str;
#else
	#error Implement this
#endif
}

std::string wcharToChar(const wchar_t * wp)
{
#if defined(TINY_PLATFORM_WINDOWS)
	std::string str;
	int len = WideCharToMultiByte(CP_ACP, 0, wp, (int)wcslen(wp), NULL, 0, NULL, NULL);
	char    *m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wp, (int)wcslen(wp), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	str = m_char;
	delete m_char;
	return str;
#else
	#error Implement this
#endif
}

std::string toLower(std::string str)
{
	for(size_t i = 0; i < str.length() ; ++i)
	{
		if(str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] += 'a' - 'A';
		}
	}
	return str;
}

std::string toUpper(std::string str)
{
	for (size_t i = 0; i < str.length(); ++i)
	{
		if (str[i] >= 'a' && str[i] <= 'z')
		{
			str[i] -= 'a' - 'A';
		}
	}
	return str;
}
