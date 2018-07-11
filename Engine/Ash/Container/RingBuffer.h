#pragma once
#include <cstdint>
#include <initializer_list>
#include <atomic>
template<class ElementType>
class RingBuffer;

/*
 *  You can use iterator like any STL iterator:
 *  
 *  RingBuffer<int> rb = {1,2,3}; //Construct a ring buffer with 3 elements
 *  int i = 0;
 *  for(RingBuffer<int>::iterator it = rb.begin(); it != rb.end(); ++it)
 *  {
 *		assert(*it == rb.get(i));
 *		++i;
 *  }
 *  
 *  What's more, you can use ringBegin() to get an iterator that will not equal to end() after the last element
 *  It will back to begin().
 *  
 *    RingBuffer<int> rb = {1,2,3}; //Construct a ring buffer with 3 elements
 *    RingBuffer<int>::ringIterator ringIt = rb.ringBegin();
 *    while(true)
 *    {
 *	      printf(*ringIt);
 *        ringit++; //will never crash, just endless loop 1 -> 2 -> 3 -> 1 -> 2 ->...
 *    }
*/
template<class ElementType, bool IsRingIter = false>
class RingBufferIterator
{
	template<class ElementType>
	friend class RingBuffer;
public:
	RingBufferIterator() = default;
	explicit RingBufferIterator(RingBuffer<ElementType>* ringBuffer);
	RingBufferIterator(const RingBufferIterator& other) = default;

	RingBufferIterator& operator=(const RingBufferIterator& other) = default;
	ElementType& operator*() const;
	ElementType* operator->() const;
	RingBufferIterator& operator++();
	RingBufferIterator& operator--();
	RingBufferIterator operator++(int);
	RingBufferIterator operator--(int);
	bool operator==(const RingBufferIterator& other);
	bool operator!=(const RingBufferIterator& other);
protected:
	RingBuffer<ElementType>* _ringBuffer = nullptr; 
	int32_t _index = 0;
};

template<class ElementType, bool IsRingIter = false>
class RingBufferConstIterator
{
	template<class ElementType>
	friend class RingBuffer;
public:
	RingBufferConstIterator() = default;
	explicit RingBufferConstIterator(const RingBuffer<ElementType>* ringBuffer);
	RingBufferConstIterator(const RingBufferConstIterator& other) = default;

	RingBufferConstIterator& operator=(const RingBufferConstIterator& other) = default;
	const ElementType& operator*() const;
	const ElementType* operator->() const;
	RingBufferConstIterator& operator++();
	RingBufferConstIterator& operator--();
	RingBufferConstIterator operator++(int);
	RingBufferConstIterator operator--(int);
	bool operator==(const RingBufferConstIterator& other);
	bool operator!=(const RingBufferConstIterator& other);
protected:
	const RingBuffer<ElementType>* _ringBuffer = nullptr;
	int32_t _index = 0;
};

template<class ElementType>
class RingBuffer
{
public:
	template<class EleType, bool IsRingIter>
	friend class RingBufferIterator;
	using iterator = RingBufferIterator<ElementType, false>;
	using ringIterator = RingBufferIterator<ElementType, true>;

	template<class EleType, bool IsRingIter>
	friend class RingBufferConstIterator;
	using constIterator = RingBufferConstIterator<ElementType, false>;
	using constRingIterator = RingBufferConstIterator<ElementType, true>;
public:
	explicit RingBuffer(int32_t initialCapacity = 0);
	explicit RingBuffer(std::initializer_list<ElementType> initList);
	RingBuffer(const RingBuffer& other);
	RingBuffer(RingBuffer&& other) noexcept;
	RingBuffer& operator=(const RingBuffer& other);
	RingBuffer& operator=(RingBuffer&& other) noexcept;
	virtual ~RingBuffer();
public:
	// Construct an element in-place and put it to the back of the RingBuffer
	template<class... Params>
	ElementType& emplaceBackItem(Params&&... params);

	// Construct elementNum elements in-place and put it to the back of the RingBuffer
	template<class... Params>
	void emplaceBackItems(int32_t elementNum, Params&&... params);

	// Return the ref of the last element
	ElementType& back();

	// pop the last element
	void popBack();

	// Construct an element in-place and put it to the front of the RingBuffer
	template<class... Params>
	ElementType& emplaceFrontItem(Params&&... params);

	// Construct elementNum elements in-place and put it to the front of the RingBuffer
	template<class... Params>
	void emplaceFrontItems(int32_t elementNum, Params&&... params);

	// Return the ref of the front element
	ElementType& front();

	// Pop the front element
	void popFront();

	// Get the element of index from front element. index start from 0
	ElementType& get(int32_t index);
	const ElementType& get(int32_t index) const;
public:
	// Get elements number
	int32_t getSize() const;

	// Get capacity
	int32_t getCapacity() const;

	// Get how many elements you can put into the buffer. It's equal to (getCapacity() - getSize())
	int32_t getFreeCount() const;

	// Change the capacity of ring buffer, you can only make capacity bigger. The memory will realloc if needed.
	void setCapacity(int32_t newSize);

	// Destruct all items, delete the capacity memory. See also clear()
	void reset();

	// Destruct all items, keep the capacity memory. See also reset()
	void clear();

	// Return if the capacity == size
	bool isFull() const;

	// copy from another RingBuffer
	void copy(const RingBuffer& other);

	// move from another RingBuffer which will be reset
	void move(RingBuffer&& other);

	void swap(RingBuffer& other) noexcept;

	iterator begin();
	ringIterator ringBegin();
	iterator end();

	constIterator begin() const;
	constRingIterator ringBegin() const;
	constIterator end() const;
protected:
	ElementType* _data = nullptr;
	int32_t _capacity = 0;
	int32_t _front = 0;
	int32_t _back = 0;
	std::atomic<int32_t> _size = 0; // In ThreadPool we need an atomic size to decide which Thread we will add task to
};

#include "RingBuffer.inl"

