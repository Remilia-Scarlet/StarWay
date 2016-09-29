#pragma once
#include <type_traits>
#include <memory.h>
#include "TinyEngine\Engine\EngineDefs.h"
template<class ValueType, int Size>
class VectorStorage
{
public:
	inline VectorStorage();
	inline VectorStorage(const ValueType& x, const ValueType& y);
	inline VectorStorage(const ValueType& x, const ValueType& y, const ValueType& z);
	inline VectorStorage(const ValueType& x, const ValueType& y, const ValueType& z, const ValueType& w);
	~VectorStorage() {}

public:
	inline ValueType& X();
	inline const ValueType& X() const;
	inline ValueType& Y();
	inline const ValueType& Y() const;
	inline ValueType& Z();
	inline const ValueType& Z() const;
	inline ValueType& W();
	inline const ValueType& W() const;

	inline ValueType& operator()(int index);
	inline const ValueType& operator()(int index) const;

	inline VectorStorage& operator=(const VectorStorage& other);
	inline VectorStorage& operator=(const ValueType& value);

	inline void setValue(int index, const ValueType& value);
	inline ValueType dot(const VectorStorage& other) const;
	inline VectorStorage& scale(const ValueType& scale);
protected:
	ValueType _data[Size];
};

template<int Size>
bool operator==(const VectorStorage<float, Size>& left, const VectorStorage<float, Size>& right)
{
	for (int i = 0; i < Size; ++i)
	{
		if (TINY_FLOAT_EQUAL(left(i), right(i)))
			return false;
	}
	return true;
}

template<class ValueType, int Size>
bool operator==(const VectorStorage<ValueType, Size>& left, const VectorStorage<ValueType, Size>& right)
{
	return memcmp(_data, other._data, sizeof(_data)) == 0;
}

template<class ValueType, int Size>
ValueType operator*(const VectorStorage<ValueType, Size>& left, const VectorStorage<ValueType, Size>& right)
{
	return left.dot(right);
}

template<class ValueType, int Size>
VectorStorage<ValueType, Size>& operator*(const VectorStorage<ValueType, Size>& vec, const ValueType& scale)
{
	return vec.scale(scale);
}
template<class ValueType, int Size>
VectorStorage<ValueType, Size>& operator*(const ValueType& scale, const VectorStorage<ValueType, Size>& vec)
{
	return vec.scale(scale);
}

#include "Vector.inl"

typedef VectorStorage<float, 2> Vector2;
typedef VectorStorage<float, 3> Vector3;
typedef VectorStorage<float, 4> Vector4;