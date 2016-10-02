//////////////////////////////////////////////////////////////////////////
// Global function
//////////////////////////////////////////////////////////////////////////
template <int Size>
bool operator==(const VectorStorage<float, Size>& left, const VectorStorage<float, Size>& right)
{
	for (int i = 0; i < Size; ++i)
	{
		if (TINY_FLOAT_EQUAL(left(i), right(i)))
			return false;
	}
	return true;
}

template <class ValueType, int Size>
bool operator==(const VectorStorage<ValueType, Size>& left, const VectorStorage<ValueType, Size>& right)
{
	return memcmp(_data, other._data, sizeof(_data)) == 0;
}

template <class ValueType, int Size>
ValueType operator*(const VectorStorage<ValueType, Size>& left, const VectorStorage<ValueType, Size>& right)
{
	return left.dot(right);
}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& operator*(const VectorStorage<ValueType, Size>& vec, const ValueType& scale)
{
	return vec.scale(scale);
}
template <class ValueType, int Size>
VectorStorage<ValueType, Size>& operator*(const ValueType& scale, const VectorStorage<ValueType, Size>& vec)
{
	return vec.scale(scale);
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
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::scaled(const ValueType& scale)
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
VectorStorage<ValueType, Size> VectorStorage<ValueType, Size>::normalized()
{

}

template <class ValueType, int Size>
VectorStorage<ValueType, Size>& VectorStorage<ValueType, Size>::normalizeInPlace()
{

}

template <class ValueType, int Size>
int VectorStorage<ValueType, Size>::lenth()
{

}
