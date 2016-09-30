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
	inline VectorStorage& operator=(const ValueType* data);

	inline void setValue(int index, const ValueType& value);
	inline ValueType dot(const VectorStorage& other) const;
	inline VectorStorage& scale(const ValueType& scale);
	inline VectorStorage& normalize();
protected:
	ValueType _data[Size];
};

#include "Vector.inl"

typedef VectorStorage<float, 2> Vector2;
typedef VectorStorage<float, 3> Vector3;
typedef VectorStorage<float, 4> Vector4;