#pragma once
#define _CRT_SECURE_NO_WARNINGS

static const int NO_LOG_FILE = 0;

#include <string>
#include <vector>
void DebugString(std::string format, ...);
std::string FormatString(const char* format, ...);
std::vector<std::string> split(const std::string& src, const std::string& separator);
std::string& replaceSubstr(std::string& src, const std::string& searchKey, const std::string& targetStr);