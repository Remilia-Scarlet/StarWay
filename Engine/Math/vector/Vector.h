#pragma once
#include <type_traits>
#include <memory.h>
#include <cmath>
#include "Math/MathDef.h"
#include "Math/quaternion/Quaternion.h"

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

	// Only for Vector2
	inline VectorStorage(const ValueType& x, const ValueType& y);

	// Only for Vector3
	inline VectorStorage(const ValueType& x, const ValueType& y, const ValueType& z);

	// Only for Vector4
	inline VectorStorage(const ValueType& x, const ValueType& y, const ValueType& z, const ValueType& w);

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

	// Multiply. return Vector{ a[0] * b[0], a[1] * b[1] , ... , a[Size - 1] * b[Size - 1] }. See also dot(), cross() and multiplyInPlace().
	inline VectorStorage multiply(const VectorStorage& other) const;

	// Multiply in self. self = Vector{ a[0] * b[0], a[1] * b[1] , ... , a[Size - 1] * b[Size - 1] }. See also dot(), cross() and multiply().
	inline VectorStorage& multiplyInPlace(const VectorStorage& other);

	// Cross multiply and return the copy. See also crossInPlace()
	inline VectorStorage cross(const VectorStorage& other) const;

	// Cross multiply in this and return this. See also cross()
	inline VectorStorage& crossInPlace(const VectorStorage& other);

	// Add a vector. Return copy. See also addInPlace()
	inline VectorStorage add(const VectorStorage& other)const;

	// Add a vector to self. Return self. See also add()
	inline VectorStorage& addInPlace(const VectorStorage& other);

	// Minus a vector. Return copy. See also minusInPlace()
	inline VectorStorage minus(const VectorStorage& other)const;

	// Minus a vector from self. Return self. See also minus()
	inline VectorStorage minusInPlace(const VectorStorage& other);

	// Multiply all elem with this value, and return the copy. See also scaleInPlace().
	inline VectorStorage scaled(const ValueType& scale) const;

	// Multiply all elem with this value, and return this. See also scaleInPlace().
	inline VectorStorage& scaleInPlace(const ValueType& scale);

	// Return a copy of normalized vector. See also normalizeInPlace().
	inline VectorStorage normalized() const;

	// Normalize vector and return ref. See also normalized()
	inline  VectorStorage& normalizeInPlace();

	// Get vector lenth. See also lenth2()
	inline ValueType lenth() const;

	// Get square of vector lenth. See also lenth()
	inline ValueType lenth2() const;

	inline ValueType& X();
	inline const ValueType& X() const;

	inline ValueType& Y();
	inline const ValueType& Y() const;

	inline ValueType& Z();
	inline const ValueType& Z() const;

	inline ValueType& W();
	inline const ValueType& W() const;

	// Rotate a copy vector by quaternion. See also rotateInPlace(). Notice : you must give a normorized quaternion.
	inline VectorStorage<ValueType, Size> rotate(const QuaternionStorage<ValueType>& normedQuaternion) const;

	// Rotate this vector bu quaternion in place. See also rotate(). Notice : you must give a normorized quaternion.
	inline VectorStorage<ValueType, Size>& rotateInPlace(const QuaternionStorage<ValueType>& normedQuaternion);

	// Return the quaternion witch can rotate this vector to other vector. Then you can use Vecor.rotate(quaternion) to rotate vector.
	inline QuaternionStorage<ValueType> getRotationToAnother(const VectorStorage<ValueType, Size>& other) const;

	// Check equal
	inline bool equal(const VectorStorage<ValueType, Size>& other) const;

	// Get a part of vector. Example : Vector4 vec(1,2,3,4); Vecter3 subVec = vec.subVecter<1,3>(); //subVec == {2,3,4}
	template<int From, int NewSize>
	inline VectorStorage<ValueType, NewSize> subVecter() const;
protected:
	ValueType _data[Size];
};

//////////////////////////////////////////////////////////////////////////
// Global function
//////////////////////////////////////////////////////////////////////////
template <class ValueType, int Size>
bool operator==(const VectorStorage<ValueType, Size>& left, const VectorStorage<ValueType, Size>& right);

// dot
template <class ValueType, int Size>
ValueType operator*(const VectorStorage<ValueType, Size>& left, const VectorStorage<ValueType, Size>& right);

// scale
template <class ValueType, int Size>
VectorStorage<ValueType, Size> operator*(const VectorStorage<ValueType, Size>& vec, const ValueType& scale);
template <class ValueType, int Size>
VectorStorage<ValueType, Size> operator*(const ValueType& scale, const VectorStorage<ValueType, Size>& vec);

// plus
template <class ValueType, int Size>
VectorStorage<ValueType, Size> operator+(const VectorStorage<ValueType, Size>& vec1, const VectorStorage<ValueType, Size>& vec2);

// minus
template <class ValueType, int Size>
VectorStorage<ValueType, Size> operator-(const VectorStorage<ValueType, Size>& vec1, const VectorStorage<ValueType, Size>& vec2);
//////////////////////////////////////////////////////////////////////////

#include "Vector.inl"

typedef VectorStorage<float, 2> Vector2;
typedef VectorStorage<float, 3> Vector3;
typedef VectorStorage<float, 4> Vector4;