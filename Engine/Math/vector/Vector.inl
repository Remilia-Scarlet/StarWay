template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage()
{
	memset(this, 0, sizeof(VectorStorage<ValueType, Size>));
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage(const ValueType& x, const ValueType& y)
{
	static_assert(Size == 2, "This constructor is only aviliable for Vector2");
	_data[0] = x;
	_data[1] = y;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage(const ValueType& x, const ValueType& y, const ValueType& z)
{
	static_assert(Size == 3, "This constructor is only aviliable for Vector3");
	_data[0] = x;
	_data[1] = y;
	_data[2] = z;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>::VectorStorage(const ValueType& x, const ValueType& y, const ValueType& z, const ValueType& w)
{
	static_assert(Size == 3, "This constructor is only aviliable for Vector3");
	_data[0] = x;
	_data[1] = y;
	_data[2] = z;
	_data[3] = w;
}

template <class ValueType, int Size>
ValueType& VectorStorage<ValueType, Size>::X()
{
	static_assert(Size >= 1, "This function is not availiable for Size < 1");
	return _data[0];
}
template <class ValueType, int Size>
const ValueType& VectorStorage<ValueType, Size>::X() const
{
	static_assert(Size >= 1, "This function is not availiable for Size < 1");
	return _data[0];
}
template <class ValueType, int Size>
ValueType& VectorStorage<ValueType, Size>::Y()
{
	static_assert(Size >= 2, "This function is not availiable for Size < 2");
	return _data[1];
}
template <class ValueType, int Size>
const ValueType& VectorStorage<ValueType, Size>::Y() const
{
	static_assert(Size >= 2, "This function is not availiable for Size < 2");
	return _data[1];
}

template <class ValueType, int Size>
ValueType& VectorStorage<ValueType, Size>::Z()
{
	static_assert(Size >= 3, "This function is not availiable for Vector2");
	return _data[2];
}
template <class ValueType, int Size>
const ValueType& VectorStorage<ValueType, Size>::Z() const
{
	static_assert(Size >= 3, "This function is not availiable for Vector2");
	return _data[2];
}

template <class ValueType, int Size>
ValueType& VectorStorage<ValueType, Size>::W()
{
	static_assert(Size >= 4, "This function is not availiable for Vector2 and Vector3");
	return _data[3];
}
template <class ValueType, int Size>
const ValueType& VectorStorage<ValueType, Size>::W() const
{
	static_assert(Size >= 4, "This function is not availiable for Vector2 and Vector3");
	return _data[3];
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
void VectorStorage<ValueType, Size>::setValue(int index, const ValueType& value)
{
	TinyAssert(index < Size && index >= 0);
	_data[index] = value;
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
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::scale(const ValueType& scale)
{
	for (i = 0; i < Size; ++i)
	{
		_data[i] *= scale;
	}
	return *this;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::operator=(const VectorStorage& other)
{
	memcpy(_data, other._data, sizeof(_data));
	return *this;
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::operator=(const ValueType& value)
{
	for (int i = 0; i < Size; ++i)
	{
		_data[i] = value;
	}
	return *this;
}
