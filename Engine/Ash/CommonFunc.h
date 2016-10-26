#pragma once
#include <string>
#include <vector>
void DebugString(std::string format, ...);
std::string FormatString(const char* format, ...);
std::vector<std::string> split(const std::string& src, const std::string& separator);