#pragma once
#include "Ash\RefCountPointer\RefCountPtr.h"

class LuaVal
{
public:
	LuaVal();							//nil
	LuaVal(bool b);						//boolean
	LuaVal(int64_t i);					//int64
	LuaVal(double d);					//number
	LuaVal(const char* s);				//string
	LuaVal(const std::string& s);		//string
	LuaVal(const RefCountObj& obj);		//obj
	template<class T>
	LuaVal(const RefCountPtr<T>& obj);	//obj
};