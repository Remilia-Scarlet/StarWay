#pragma once
#define _CRT_SECURE_NO_WARNINGS

constexpr bool NO_LOG_FILE = 1;

#include <string>
#include <vector>
void DebugString(const char* format, ...);
std::string FormatString(const char* format, ...);
std::vector<std::string> split(const std::string& src, const std::string& separator);
std::string& replaceSubstr(std::string& src, const std::string& searchKey, const std::string& targetStr);
std::wstring charToWchar(const char* c);
std::string wcharToChar(const wchar_t* wp);
std::string toLower(std::string str);
std::string toUpper(std::string str);

#define TINY_BREAK_IF(CENTENCE) if(CENTENCE)break
#define TINY_SAFE_DELETE(PTR) do{if(PTR != nullptr){delete PTR;PTR = nullptr;}}while(0)
#define TINY_SAFE_RELEASE(PTR) do{if(PTR != nullptr){PTR->Release();PTR = nullptr;}}while(0)
#define TO_STRING(_NAME_) (#_NAME_)

typedef int64_t ObjectID;
const static ObjectID INVALID_OBJECT_ID = 0;


#define TINY_DEFINE_PTR(CALSSNAME) class CALSSNAME;typedef RefCountPtr<CALSSNAME> CALSSNAME##Ptr;typedef WeakRefPtr<CALSSNAME> CALSSNAME##WeakPtr