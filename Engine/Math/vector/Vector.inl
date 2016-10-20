#include "TinyEngine/Engine/TinyAssert.h"
//////////////////////////////////////////////////////////////////////////
// Global function
//////////////////////////////////////////////////////////////////////////
template <class ValueType, int Size>
bool operator==(const VectorStorage<ValueType, Size>& left, const VectorStorage<ValueType, Size>& right)
{
	return left.equal(right);
}

template <class ValueType, int Size>
ValueType operator*(const VectorStorage<ValueType, Size>& left, const VectorStorage<ValueType, Size>& right)
{
	return left.dot(right);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size> operator*(const VectorStorage<ValueType, Size>& vec, const ValueType& scale)
{
	return vec.scaled(scale);
}
template <class ValueType, int Size>
VectorStorage<ValueType, Size> operator*(const ValueType& scale, const VectorStorage<ValueType, Size>& vec)
{
	return vec.scaled(scale);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size> operator+(const VectorStorage<ValueType, Size>& vec1, const VectorStorage<ValueType, Size>& vec2)
{
	return vec1.add(vec2);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size> operator-(const VectorStorage<ValueType, Size>& vec1, const VectorStorage<ValueType, Size>& vec2)
{
	return vec1.minus(vec2);
}
//////////////////////////////////////////////////////////////////////////
// Member function
//////////////////////////////////////////////////////////////////////////
template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage()
{
	reset();
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage(std::initializer_list<ValueType> valueList)
{
	reset(valueList);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage(const ValueType* data)
{
	reset(data);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage(const ValueType& value)
{
	reset(value);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage(const VectorStorage& other)
{
	reset(other);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage(const ValueType& x, const ValueType& y)
{
	static_assert(Size == 2, "This constructor is only available for Vector2");
	_data[0] = x;
	_data[1] = y;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage(const ValueType& x, const ValueType& y, const ValueType& z)
{
	static_assert(Size == 3, "This constructor is only available for Vector3");
	_data[0] = x;
	_data[1] = y;
	_data[2] = z;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage(const ValueType& x, const ValueType& y, const ValueType& z, const ValueType& w)
{
	static_assert(Size == 4, "This constructor is only available for Vector4");
	_data[0] = x;
	_data[1] = y;
	_data[2] = z;
	_data[3] = w;
}

template <class ValueType, int Size>
ValueType& VectorStorage<ValueType, Size>::operator()(int index)
{
	TinyAssert(index < Size && index >= 0);
	return _data[index];
}

template <class ValueType, int Size>
const ValueType& VectorStorage<ValueType, Size>::operator()(int index) const
{
	TinyAssert(index < Size && index >= 0);
	return _data[index];
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::operator=(std::initializer_list<ValueType> valueList)
{
	reset(valueList);
	return *this;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::operator=(const ValueType* data)
{
	reset(data);
	return *this;
}
template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::operator=(const ValueType& value)
{
	reset(value);
	return *this;
}
template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::operator=(const VectorStorage& other)
{
	reset(other);
	return *this;
}

template <class ValueType, int Size>
void VectorStorage<ValueType, Size>::reset()
{
	memset(_data, 0, sizeof(_data));
}

template <class ValueType, int Size>
void VectorStorage<ValueType, Size>::reset(std::initializer_list<ValueType> valueList)
{
	for (int i = 0; i < Size; ++i)
	{
		if (i < (int)valueList.size())
			_data[i] = *(valueList.begin() + i);
		else
			_data[i] = 0;
	}
}

template <class ValueType, int Size>
void VectorStorage<ValueType, Size>::reset(const ValueType* data)
{
	memcpy(_data, data, sizeof(_data));
}

template <class ValueType, int Size>
void VectorStorage<ValueType, Size>::reset(const ValueType& value)
{
	for (int i = 0; i < Size; ++i)
	{
		_data[i] = value;
	}
}

template <class ValueType, int Size>
void VectorStorage<ValueType, Size>::reset(const VectorStorage& other)
{
	memcpy(_data, other._data, sizeof(_data));
}

template <class ValueType, int Size>
void VectorStorage<ValueType, Size>::setValue(int index, const ValueType& value)
{
	TinyAssert(index < Size && index >= 0);
	_data[index] = value;
}

template <class ValueType, int Size>
ValueType* VectorStorage<ValueType, Size>::getData()
{
	return _data;
}

template <class ValueType, int Size>
const ValueType* VectorStorage<ValueType, Size>::getData() const
{
	return _data;
}

template <class ValueType, int Size>
ValueType VectorStorage<ValueType, Size>::dot(const VectorStorage& other) const
{
	ValueType sum = 0;
	for (int i = 0; i < Size; ++i)
	{
		sum += _data[i] * other._data[i];
	}
	return sum;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size> VectorStorage<ValueType, Size>::multiply(const VectorStorage<ValueType, Size>& other) const
{
	VectorStorage<ValueType, Size> vec = *this;
	return vec.multiplyInPlace(other);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::multiplyInPlace(const VectorStorage<ValueType, Size>& other)
{
	for (int i = 0; i < Size; ++i)
	{
		_data[i] *= other._data[i];
	}
	return *this;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size> VectorStorage<ValueType, Size>::cross(const VectorStorage<ValueType, Size>& other) const
{
	VectorStorage<ValueType, Size> ret = *this;
	return ret.crossInPlace(other);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::crossInPlace(const VectorStorage<ValueType, Size>& other)
{
	static_assert(Size == 3, "Only Vector3 can use cross");

	ValueType x1 = X(), y1 = Y(), z1 = Z();
	ValueType x2 = other.X(), y2 = other.Y(), z2 = other.Z();

	X() = y1 * z2 - z1 * y2;
	Y() = x2 * z1 - z2 * x1;
	Z() = x1 * y2 - x2 * y1;
	return *this;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size> VectorStorage<ValueType, Size>::add(const VectorStorage<ValueType, Size>& other) const
{
	return VectorStorage<ValueType, Size>(*this).addInPlace(other);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::addInPlace(const VectorStorage<ValueType, Size>& other)
{
	for (int i = 0; i < Size; ++i)
	{
		_data[i] += other._data[i];
	}
	return *this;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size> VectorStorage<ValueType, Size>::minus(const VectorStorage<ValueType, Size>& other) const
{
	return VectorStorage<ValueType, Size>(*this).minusInPlace(other);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size> VectorStorage<ValueType, Size>::minusInPlace(const VectorStorage<ValueType, Size>& other) 
{
	for (int i = 0; i < Size; ++i)
	{
		_data[i] -= other._data[i];
	}
	return *this;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size> VectorStorage<ValueType, Size>::scaled(const ValueType& scale) const
{
	VectorStorage<ValueType, Size> target = *this;
	for (int i = 0; i < Size; ++i)
	{
		target._data[i] *= scale;
	}
	return target;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::scaleInPlace(const ValueType& scale)
{
	for (int i = 0; i < Size; ++i)
	{
		_data[i] *= scale;
	}
	return *this;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size> VectorStorage<ValueType, Size>::normalized() const
{
	VectorStorage<ValueType, Size> ret = *this;
	return ret.normalizeInPlace();
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::normalizeInPlace()
{
	ValueType len = lenth();
	for (int i = 0; i < Size; ++i)
	{
		_data[i] /= len;
	}
	return *this;
}

template <class ValueType, int Size>
ValueType VectorStorage<ValueType, Size>::lenth() const
{
	return sqrt(lenth2());
}

template <class ValueType, int Size>
ValueType VectorStorage<ValueType, Size>::lenth2() const
{
	return this->dot(*this);
}

template <class ValueType, int Size>
ValueType& VectorStorage<ValueType, Size>::X()
{
	static_assert(Size >= 1, "This is only aviliable when Size >= 1");
	return _data[0];
}

template <class ValueType, int Size>
const ValueType& VectorStorage<ValueType, Size>::X() const
{
	static_assert(Size >= 1, "This is only aviliable when Size >= 1");
	return _data[0];
}

template <class ValueType, int Size>
ValueType& VectorStorage<ValueType, Size>::Y()
{
	static_assert(Size >= 2, "This is only aviliable when Size >= 2");
	return _data[1];
}

template <class ValueType, int Size>
const ValueType& VectorStorage<ValueType, Size>::Y() const
{
	static_assert(Size >= 2, "This is only aviliable when Size >= 2");
	return _data[1];
}

template <class ValueType, int Size>
ValueType& VectorStorage<ValueType, Size>::Z()
{
	static_assert(Size >= 3, "This is only aviliable when Size >= 3");
	return _data[2];
}

template <class ValueType, int Size>
const ValueType& VectorStorage<ValueType, Size>::Z() const
{
	static_assert(Size >= 3, "This is only aviliable when Size >= 3");
	return _data[2];
}

template <class ValueType, int Size>
ValueType& VectorStorage<ValueType, Size>::W()
{
	static_assert(Size >= 4, "This is only aviliable when Size >= 4");
	return _data[3];
}

template <class ValueType, int Size>
const ValueType& VectorStorage<ValueType, Size>::W() const
{
	static_assert(Size >= 4, "This is only aviliable when Size >= 4");
	return _data[3];
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size> VectorStorage<ValueType, Size>::rotate(const QuaternionStorage<ValueType>& normedQuaternion) const
{
	VectorStorage<ValueType, Size> ret = *this;
	return ret.rotateInPlace(normedQuaternion);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::rotateInPlace(const QuaternionStorage<ValueType>& normedQuaternion)
{
	static_assert(Size == 3, "Only Vector3 can be rotated by quaternion");
	TinyAssert(normedQuaternion.isNormalized(), "You must use normalized quaternion to rotate a vector");

	VectorStorage<ValueType, Size> qv = { normedQuaternion.X(), normedQuaternion.Y(), normedQuaternion.Z() };
	VectorStorage<ValueType, Size>& v = *this;

	*this = v + ((ValueType)2 * qv).cross(qv.cross(v) + normedQuaternion.W() * v);
	return *this;
}

template <class ValueType, int Size>
QuaternionStorage<ValueType> VectorStorage<ValueType, Size>::getRotationToAnother(const VectorStorage<ValueType, Size>& other) const
{
	return QuaternionStorage<ValueType>(*this, other);
}

template <class ValueType, int Size>
bool VectorStorage<ValueType, Size>::equal(const VectorStorage<ValueType, Size>& other) const
{
	for (int i = 0; i < Size; ++i)
	{
		if (!isEqual(_data[i], other._data[i]))
			return false;
	}
	return true;
}

template<class ValueType, int Size>
template<int From, int NewSize>
inline VectorStorage<ValueType, NewSize> VectorStorage<ValueType, Size>::subVecter() const
{
	static_assert(From < Size, "How can From >= Size??");
	const int realSize = (NewSize <= Size - From ? NewSize : Size - From);
	VectorStorage<ValueType, NewSize> newVec = std::initializer_list<ValueType>(_data + From, _data + From + realSize);
	return newVec;
}