#pragma once

#include <streambuf>

// Use an array as stream target
class ArrayStream
{	
	enum
	{	// constants for bits in stream state
		_Constant = 2,	// set if character array nonmutable
		_Noread = 4,	// set if character array cannot be read
	};	// set if initial writes are appends
	typedef int _Strstate;
public:
	typedef std::streambuf _Mysb;
	typedef std::streambuf::traits_type _Traits;
	typedef char _Elem;

	ArrayStream()
	{
		
	}

	void initAsInputStream(const _Elem* data, size_t size)
	{
		
	}
	void initAsOutputStream(_Elem* buffer, size_t size)
	{
		
	}

	ArrayStream(const ArrayStream&) = delete;
	ArrayStream& operator=(const ArrayStream&) = delete;

	virtual ~ArrayStream()
	{	
	}


protected:
	

private:
	
};
