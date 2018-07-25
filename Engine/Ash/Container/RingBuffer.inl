
#include "Ash/TinyAssert.h"
#include "RingBuffer.h"

template <class ElementType, bool IsRingIter>
RingBufferIterator<ElementType, IsRingIter>::RingBufferIterator(RingBuffer<ElementType>* ringBuffer)
	:_ringBuffer(ringBuffer)
{

}

template <class ElementType, bool IsRingIter>
ElementType& RingBufferIterator<ElementType, IsRingIter>::operator*() const
{
	if(_ringBuffer == 0)
	{
		TinyAssert(false);
		ElementType* nullPtr = nullptr;
		return reinterpret_cast<ElementType&>(*nullPtr); //crash
	}
	return _ringBuffer->get(_index);
}

template <class ElementType, bool IsRingIter>
ElementType* RingBufferIterator<ElementType, IsRingIter>::operator->() const
{
	if (_ringBuffer == 0)
	{
		TinyAssert(false);
		return nullptr;
	}
	return &_ringBuffer->get(_index);
}

template <class ElementType, bool IsRingIter>
RingBufferIterator<ElementType, IsRingIter>& RingBufferIterator<ElementType, IsRingIter>::operator++()
{
	TinyAssert(_ringBuffer);
	++_index;
	if(IsRingIter)
	{
		_index %= _ringBuffer->_capacity;
	}
	return *this;
}

template <class ElementType, bool IsRingIter>
RingBufferIterator<ElementType, IsRingIter>& RingBufferIterator<ElementType, IsRingIter>::operator--()
{
	TinyAssert(_ringBuffer);
	--_index;
	if(IsRingIter)
	{
		if (_index < 0)
			_index += _ringBuffer->_capacity;
	}
	return *this;
}

template <class ElementType, bool IsRingIter>
RingBufferIterator<ElementType, IsRingIter> RingBufferIterator<ElementType, IsRingIter>::operator++(int)
{
	TinyAssert(_ringBuffer);
	RingBufferIterator keepOld = *this;
	++_index;
	if (IsRingIter)
	{
		_index %= _ringBuffer->_capacity;
	}
	return keepOld;
}

template <class ElementType, bool IsRingIter>
RingBufferIterator<ElementType, IsRingIter> RingBufferIterator<ElementType, IsRingIter>::operator--(int)
{
	TinyAssert(_ringBuffer);
	RingBufferIterator keepOld = *this;
	--_index;
	if (IsRingIter)
	{
		if (_index < 0)
			_index += _ringBuffer->_capacity;
	}
	return keepOld;
}

template <class ElementType, bool IsRingIter>
bool RingBufferIterator<ElementType, IsRingIter>::operator==(const RingBufferIterator& other)
{
	return _ringBuffer == other._ringBuffer && _index == other._index;
}

template <class ElementType, bool IsRingIter>
bool RingBufferIterator<ElementType, IsRingIter>::operator!=(const RingBufferIterator& other)
{
	return !(*this == other);
}

template <class ElementType, bool IsRingIter>
RingBufferConstIterator<ElementType, IsRingIter>::RingBufferConstIterator(const RingBuffer<ElementType>* ringBuffer)
	:_ringBuffer(ringBuffer)
{

}

template <class ElementType, bool IsRingIter>
const ElementType& RingBufferConstIterator<ElementType, IsRingIter>::operator*() const
{
	if (_ringBuffer == 0)
	{
		TinyAssert(false);
		ElementType* nullPtr = nullptr;
		return reinterpret_cast<ElementType&>(*nullPtr); //crash
	}
	return _ringBuffer->get(_index);
}

template <class ElementType, bool IsRingIter>
const ElementType* RingBufferConstIterator<ElementType, IsRingIter>::operator->() const
{
	if (_ringBuffer == 0)
	{
		TinyAssert(false);
		return nullptr;
	}
	return &_ringBuffer->get(_index);
}

template <class ElementType, bool IsRingIter>
RingBufferConstIterator<ElementType, IsRingIter>& RingBufferConstIterator<ElementType, IsRingIter>::operator++()
{
	TinyAssert(_ringBuffer);
	++_index;
	if (IsRingIter)
	{
		_index %= _ringBuffer->_capacity;
	}
	return *this;
}

template <class ElementType, bool IsRingIter>
RingBufferConstIterator<ElementType, IsRingIter>& RingBufferConstIterator<ElementType, IsRingIter>::operator--()
{
	TinyAssert(_ringBuffer);
	--_index;
	if (IsRingIter)
	{
		if (_index < 0)
			_index += _ringBuffer->_capacity;
	}
	return *this;
}

template <class ElementType, bool IsRingIter>
RingBufferConstIterator<ElementType, IsRingIter> RingBufferConstIterator<ElementType, IsRingIter>::operator++(int)
{
	TinyAssert(_ringBuffer);
	RingBufferConstIterator keepOld = *this;
	++_index;
	if (IsRingIter)
	{
		_index %= _ringBuffer->_capacity;
	}
	return keepOld;
}

template <class ElementType, bool IsRingIter>
RingBufferConstIterator<ElementType, IsRingIter> RingBufferConstIterator<ElementType, IsRingIter>::operator--(int)
{
	TinyAssert(_ringBuffer);
	RingBufferConstIterator keepOld = *this;
	--_index;
	if (IsRingIter)
	{
		if (_index < 0)
			_index += _ringBuffer->_capacity;
	}
	return keepOld;
}

template <class ElementType, bool IsRingIter>
bool RingBufferConstIterator<ElementType, IsRingIter>::operator==(const RingBufferConstIterator& other)
{
	return _ringBuffer == other._ringBuffer && _index == other._index;
}

template <class ElementType, bool IsRingIter>
bool RingBufferConstIterator<ElementType, IsRingIter>::operator!=(const RingBufferConstIterator& other)
{
	return !(*this == other);
}

template <class ElementType>
RingBuffer<ElementType>::RingBuffer(int32_t initialCapacity)
	:_capacity(initialCapacity)
{
	if (initialCapacity > 0)
	{
		_data = reinterpret_cast<ElementType*>(new uint8_t[sizeof(ElementType) * _capacity]);//todo: align is not right
	}
}

template<class ElementType>
inline RingBuffer<ElementType>::RingBuffer(std::initializer_list<ElementType> initList)
{
	setCapacity(static_cast<int32_t>(initList.size()));
	for(auto& item : initList)
	{
		emplaceBackItem(item);
	}

}

template<class ElementType>
inline RingBuffer<ElementType>::RingBuffer(const RingBuffer & other)
{
	copy(other);
}

template <class ElementType>
RingBuffer<ElementType>::RingBuffer(RingBuffer&& other) noexcept
{
	move(std::move(other));
}

template <class ElementType>
RingBuffer<ElementType>& RingBuffer<ElementType>::operator=(const RingBuffer& other)
{
	copy(other);
	return *this;
}

template <class ElementType>
RingBuffer<ElementType>& RingBuffer<ElementType>::operator=(RingBuffer&& other) noexcept
{
	move(std::move(other));
	return *this;
}

template <class ElementType>
RingBuffer<ElementType>::~RingBuffer()
{
	reset();
}

template <class ElementType>
template <class ... Params>
ElementType& RingBuffer<ElementType>::emplaceBackItem(Params&&... params)
{
	if(isFull())
	{
		TinyAssert(false);
		ElementType* nullPtr = nullptr;
		return reinterpret_cast<ElementType&>(*nullPtr); //crash
	}
	ElementType* ele = (_data + _back);
	++_back;
	++_size;
	_back %= _capacity;
	new (ele) ElementType(std::forward<Params>(params)...); //todo: align is not right! will crash if ElementType align bigger than 8
	return *ele;
}

template <class ElementType>
template <class ... Params>
void RingBuffer<ElementType>::emplaceBackItems(int32_t elementNum, Params&&... params)
{
	if (getFreeCount() < elementNum)
	{
		TinyAssert(false);
		return;
	}
	for(int32_t i = 0 ; i < elementNum ;++i)
	{
		ElementType* ele = (_data + _back);
		++_back;
		++_size;
		_back %= _capacity;
		new (ele) ElementType(std::forward<Params>(params)...);//todo: align is not right! will crash if ElementType align bigger than 8
	}
}

template <class ElementType>
ElementType& RingBuffer<ElementType>::back()
{
	if (getSize() == 0)
	{
		TinyAssert(false);
		ElementType* nullPtr = nullptr;
		return static_cast<ElementType&>(*nullPtr); //crash
	}
	int32_t pos = _back - 1;
	if (pos < 0)
		pos += _capacity;
	return *(_data + pos);
}

template <class ElementType>
void RingBuffer<ElementType>::popBack()
{
	if (getSize() == 0)
	{
		TinyAssert(false);
		return;
	}
	back().~ElementType();
	--_back;
	--_size;
	if (_back < 0)
		_back += _capacity;
}

template <class ElementType>
template <class ... Params>
ElementType&  RingBuffer<ElementType>::emplaceFrontItem(Params&&... params)
{
	if (isFull())
	{
		TinyAssert(false);
		ElementType* nullPtr = nullptr;
		return reinterpret_cast<ElementType&>(*nullPtr); //crash
	}
	--_front;
	++_size;
	if (_front < 0)
		_front += _capacity;
	ElementType* ele = (_data + _front);
	new (ele) ElementType(std::forward<Params>(params)...);
	return *ele;
}

template <class ElementType>
template <class ... Params>
void RingBuffer<ElementType>::emplaceFrontItems(int32_t elementNum, Params&&... params)
{
	if (getFreeCount() < elementNum)
	{
		TinyAssert(false);
		return;
	}
	for (int32_t i = 0; i < elementNum; ++i)
	{
		--_front;
		++_size;
		if (_front < 0)
			_front += _capacity;
		ElementType* ele = (_data + _front);
		new (ele) ElementType(std::forward<Params>(params)...);
	}
}

template <class ElementType>
ElementType& RingBuffer<ElementType>::front()
{
	if (getSize() == 0)
	{
		TinyAssert(false);
		ElementType* nullPtr = nullptr;
		return reinterpret_cast<ElementType&>(*nullPtr); //crash
	}
	return *(_data + _front);
}

template <class ElementType>
void RingBuffer<ElementType>::popFront()
{
	if (getSize() == 0)
	{
		TinyAssert(false);
		return;
	}
	front().~ElementType();
	++_front;
	--_size;
	_front %= _capacity;
}

template <class ElementType>
ElementType& RingBuffer<ElementType>::get(int32_t index)
{
	if(index >= getSize())
	{
		TinyAssert(false);
		ElementType* nullPtr = nullptr;
		return reinterpret_cast<ElementType&>(*nullPtr); //crash
	}
	return _data[_front + index];
}

template <class ElementType>
const ElementType& RingBuffer<ElementType>::get(int32_t index) const
{
	if (index >= getSize())
	{
		TinyAssert(false);
		ElementType* nullPtr = nullptr;
		return reinterpret_cast<ElementType&>(*nullPtr); //crash
	}
	return _data[_front + index];
}

template <class ElementType>
int32_t RingBuffer<ElementType>::getSize() const
{
	return _size;
}

template <class ElementType>
int32_t RingBuffer<ElementType>::getCapacity() const
{
	return _capacity;
}

template<class ElementType>
inline int32_t RingBuffer<ElementType>::getFreeCount() const
{
	return _capacity - _size.load();
}

template <class ElementType>
void RingBuffer<ElementType>::setCapacity(int32_t newSize)
{
	if(newSize <= getSize())
		return;
	ElementType* newData = reinterpret_cast<ElementType*>(new uint8_t[sizeof(ElementType) * newSize]); //todo: align is not right
	int32_t newDataPos = 0;
	while(_size.load())
	{
		new (newData + newDataPos) ElementType{ std::move(*(_data + _front)) };
		(_data + _front)->~ElementType();
		++_front;
		--_size;
		_front %= _capacity;
		++newDataPos;
	}
	delete[] reinterpret_cast<uint8_t*>(_data);
	_data = newData;
	_capacity = newSize;
	_front = 0;
	_back = newDataPos;
	_size = newDataPos;
}

template <class ElementType>
void RingBuffer<ElementType>::reset()
{
	clear();
	delete[] reinterpret_cast<uint8_t*>(_data);
	_data = nullptr;
	_capacity = 0;
}

template <class ElementType>
void RingBuffer<ElementType>::clear()
{
	while(_size.load())
	{
		(_data + _front)->~ElementType();
		++_front;
		--_size;
		_front %= _capacity;
	}
	_front = 0;
	_back = 0;
}

template<class ElementType>
inline bool RingBuffer<ElementType>::isFull() const
{
	return _size.load() == _capacity;
}

template <class ElementType>
void RingBuffer<ElementType>::copy(const RingBuffer& other)
{
	clear();
	setCapacity(other._capacity);
	for(auto it = other.begin(); it != other.end() ;++it)
	{
		new (_data + _back++) ElementType{*it};
		++_size;
	}
}

template <class ElementType>
void RingBuffer<ElementType>::move(RingBuffer&& other)
{
	reset();
	swap(other);
}

template <class ElementType>
void RingBuffer<ElementType>::swap(RingBuffer& other) noexcept
{
	std::swap(_data, other._data);
	std::swap(_capacity, other._capacity);
	std::swap(_front, other._front);
	std::swap(_back, other._back);
	int othersize = other._size;
	other._size = _size.load();
	_size = othersize;
}

template <class ElementType>
typename RingBuffer<ElementType>::iterator RingBuffer<ElementType>::begin()
{
	return iterator(this);
}

template <class ElementType>
typename RingBuffer<ElementType>::ringIterator RingBuffer<ElementType>::ringBegin()
{
	return ringIterator(this);
}

template <class ElementType>
typename RingBuffer<ElementType>::iterator RingBuffer<ElementType>::end()
{
	iterator ret(this);
	ret._index = getSize();
	return ret;
}

template <class ElementType>
typename RingBuffer<ElementType>::constIterator RingBuffer<ElementType>::begin() const
{
	return constIterator(this);
}

template <class ElementType>
typename RingBuffer<ElementType>::constRingIterator RingBuffer<ElementType>::ringBegin() const
{
	return constRingIterator(this);
}

template <class ElementType>
typename RingBuffer<ElementType>::constIterator RingBuffer<ElementType>::end() const
{
	constIterator ret(this);
	ret._index = getSize();
	return ret;
}
