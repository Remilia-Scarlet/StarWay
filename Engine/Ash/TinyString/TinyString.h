#pragma once
#include "RefCountPointer/RefCountObj.h"

class TinySring
{
public:
	TinySring();
	TinySring(const char* str);
	TinySring(const char* str, size_t len);
	template<size_t N>
	TinySring(const char(&str)[N]);
	TinySring(const TinySring& other);
	TinySring(TinySring&& other);

protected:


};

#include "TinyString.inl"
