#pragma once
#include <type_traits>
#include <memory.h>
#include <cmath>
#include "TinyEngine\Engine\EngineDefs.h"

template <class ValueType, int Size>
class VectorStorage
{
public:
	// Construct a Vector with all elements 0.
	inline VectorStorage();

	// Initilize vector by {....}. For example:
	// VectorStorage<float,3> vector = {1,0,0};
	inline VectorStorage(std::initializer_list<ValueType> valueList);

	// Initilize vector by data.
	inline VectorStorage(const ValueType* data);

	// Initilize all elements with a value
	inline VectorStorage(const ValueType& value);

	// Copy another vector
	inline VectorStorage(const VectorStorage& other);

	inline ~VectorStorage() {}
	// Get an elem
	inline ValueType& operator()(int index);
	inline const ValueType& operator()(int index) const;

	inline VectorStorage& operator=(std::initializer_list<ValueType> valueList);
	inline VectorStorage& operator=(const ValueType* data);
	inline VectorStorage& operator=(const ValueType& value);
	inline VectorStorage& operator=(const VectorStorage& other);

	// Reset all elem to 0
	inline void reset();

	// Reset vector by {....}. For example:
	// vector = {1,0,0};
	inline void reset(std::initializer_list<ValueType> valueList);

	// Reset vector by data.
	inline void reset(const ValueType* data);

	// Reset vector by value.
	inline void reset(const ValueType& value);

	// Copy another vector
	inline void reset(const VectorStorage& other);

	// Set an elem
	inline void setValue(int index, const ValueType& value);

	// return the data
	inline ValueType* getData();
	inline const ValueType* getData() const;

	// Dot multiply
	inline ValueType dot(const VectorStorage& other) const;

	// Multiply all elem with this value, and return the copy. See also scaleInPlace().
	inline VectorStorage scaled(const ValueType& scale) const;

	// Multiply all elem with this value, and return this. See also scaleInPlace().
	inline VectorStorage& scaleInPlace(const ValueType& scale);

	// Return a copy of normalized vector. See also normalizeInPlace().
	inline VectorStorage normalized() const;

	// Normalize vector and return ref. See also normalized()
	inline  VectorStorage& normalizeInPlace();

	// Get vector lenth
	inline ValueType lenth() const;

	inline ValueType& X();
	inline const ValueType& X() const;

	inline ValueType& Y();
	inline const ValueType& Y() const;

	inline ValueType& Z();
	inline const ValueType& Z() const;

	inline ValueType& W();
	inline const ValueType& W() const;
protected:
	ValueType _data[Size];
};

#include "Vector.inl"

typedef VectorStorage<float, 2> Vector2;
typedef VectorStorage<float, 3> Vector3;
typedef VectorStorage<float, 4> Vector4;