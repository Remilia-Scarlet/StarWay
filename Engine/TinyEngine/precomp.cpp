#include "TinyEngine\precomp.h"
#include <stdarg.h>
#include <windows.h>

void DebugString(std::string format, ...)
{
	format += "\n";
	static char buffer[1024];
	va_list args;
	va_start(args, format);
	vsprintf(buffer, format.c_str(), args);
	va_end(args);
	OutputDebugStringA(buffer);
	printf(buffer);
}