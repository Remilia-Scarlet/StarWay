#include "TaskRingBuffer.h"

template<class T>
inline TaskRingBuffer<T>::TaskRingBuffer()
{
	_data = (T*)new uint8_t[INITIAL_CAPACITY * sizeof(T)]; //TODO: aligned?
	_capacity = INITIAL_CAPACITY;
}

template<class T>
inline TaskRingBuffer<T>::~TaskRingBuffer()
{
	//Set exiting flag and wait till all popping and pushing thread exiting
	setExiting();
	while (_popingThreadNum)
		std::this_thread::yield();
	while (_pushingThreadNum)
		std::this_thread::yield();

	//Destruct all elements
	int end = _back.load();
	int current = _front.load();
	while(current != end)
	{
		_data[current].~T();
		current = (current + 1) % _capacity;
	}
	delete[] (uint8_t*)(_data);
}

template<class T>
void TaskRingBuffer<T>::pushBack(T elem)
{
	++_pushingThreadNum;
	TinyAssert(_pushingThreadNum.load() == 1, "You can't call pushBack in two threads at the same time");

	TinyAssert(!_isExiting, "You can't call pushBack when you already called setExiting()");
	if (_isExiting)
		return;

	if(isFull())
	{
		_blockForFull = true;
		//wait till all popping threads are waiting or already exiting
		while (_popingThreadNum == _popingWaitingThreadNum) 
			std::this_thread::yield();
		increaseCapacity();
		_blockForFull = false;
	}
	//push back
	int nextPushPos = _front.load() + 1;
	nextPushPos %= _capacity;
	new (_data + nextPushPos)T(std::move(elem));
	++_front;
	_waitingForPushCondi.notify_one();
	--_pushingThreadNum;
}

template<class T>
T TaskRingBuffer<T>::popFront()
{
	struct NumGuard
	{
		NumGuard(std::atomic<int32_t>& num) :_num(num) { ++_num; }
		~NumGuard() { --_num; }
		std::atomic<int32_t>& _num;
	};

	NumGuard popingThreadNumGuard(_popingThreadNum);
	TinyAssert(!_isExiting);

	while (!_isExiting.load())
	{
		if (isEmpty() || _blockForFull.load())
		{
			NumGuard popingWaitingThreadNumGuard(_popingWaitingThreadNum);
			std::unique_lock<std::mutex> lock(_waitingForPushMtx);
			_waitingForPushCondi.wait([this]() {return (!isEmpty() && !_blockForFull) || _isExiting; });
		}
		else
		{
			//CAS popping the front
			int32_t currentFront = _front.load();
			int32_t currentBack = _back.load();
			if(currentBack == currentFront)
				continue;
			_front.compare_exchange_strong(currentFront, currentFront + 1);

			//Now currentFront is mine
			T ret{ std::move(_data[currentFront]) };
			_data[currentFront].~T();
			return std::move(ret);
		}
	}
	return T{};
}

template<class T>
inline void TaskRingBuffer<T>::setExiting()
{
	_isExiting = true;
	_waitingForPushCondi.notify_all();
}

template <class T>
bool TaskRingBuffer<T>::isFull() const
{
	return (_front + _capacity - _back) % _capacity <= KEEP_CAPACITY;
}

template <class T>
bool TaskRingBuffer<T>::isEmpty() const
{
	return _front == _back;
}

template<class T>
inline int32_t TaskRingBuffer<T>::getSize() const
{
	return (_back + _capacity - _front) % _capacity;
}

template<class T>
inline void TaskRingBuffer<T>::increaseCapacity()
{
	//alloc new space
	T* old = _data;
	_data = (T*)new uint8_t[int32_t(_capacity * 1.2) * sizeof(T)]; //TODO: aligned?

	//move to new space
	int end = _back.load();
	int current = _front.load();
	while (current != end)
	{
		new (_data + current) T(std::move(old[current]));
		current = (current + 1) % _capacity;
	}
	delete[] (uint8_t*)(old);
}
