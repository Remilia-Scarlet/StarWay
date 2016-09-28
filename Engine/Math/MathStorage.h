#pragma once
#include <type_traits>
#include <memory.h>
#include "TinyEngine\Engine\EngineDefs.h"
template<class ValueType, int Size>
class MathStorage
{
public:
	MathStorage()
	{
		memset(this, 0, sizeof(MathStorage<ValueType,Size>));
	}
	~MathStorage(){}

public:
	template<class = typename std::enable_if<(Size >= 1), void>::type>
	inline ValueType& X()
	{
		return _data[0];
	}
	template<class = typename std::enable_if<(Size >= 1), void>::type>
	inline const ValueType& X() const
	{
		return _data[0];
	}

	template<class = typename std::enable_if<(Size >= 2), void>::type>
	inline ValueType& Y() 
	{
		return _data[1];
	}
	template<class = typename std::enable_if<(Size >= 2), void>::type>
	inline const ValueType& Y() const
	{
		return _data[1];
	}

	template<class = typename std::enable_if<(Size >= 3), void>::type>
	inline ValueType& Z()
	{
		return _data[2];
	}
	template<class = typename std::enable_if<(Size >= 3), void>::type>
	inline const ValueType& Z() const
	{
		return _data[2];
	}

	template<class = typename std::enable_if<(Size >= 4), void>::type>
	inline ValueType& W()
	{
		return _data[3];
	}
	template<class = typename std::enable_if<(Size >= 4), void>::type>
	inline const ValueType& W() const
	{
		return _data[3];
	}

	inline ValueType& operator()(int index)
	{
		TinyAssert(index < Size && index >= 0);
		return _data[index];
	}
	inline const ValueType& operator()(int index) const
	{
		TinyAssert(index < Size && index >= 0);
		return _data[index];
	}

	inline void setValue(int index, const ValueType& value)
	{
		TinyAssert(index < Size && index >= 0);
		_data[index] = value;
	}

protected:
	ValueType _data[Size];
};

template<int Size>
bool operator==(const MathStorage<float, Size>& left, const MathStorage<float, Size>& right) 
{
	for (int i = 0; i < Size; ++i)
	{
		if (TINY_FLOAT_EQUAL(left(i),right(i)))
			return false;
	}
	return true;
}

template<class ValueType, int Size>
bool operator==(const MathStorage<ValueType, Size>& left, const MathStorage<ValueType, Size>& right) 
{
	return memcmp(_data, other._data, sizeof(_data)) == 0;
}
