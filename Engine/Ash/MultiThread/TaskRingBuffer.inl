#include "TaskRingBuffer.h"

const int INITIAL_CAPACITY = 32;

template<class T>
inline TaskRingBuffer<T>::TaskRingBuffer()
{
	_data = (T*)new uint8_t[INITIAL_CAPACITY * sizeof(T)]; //TODO: aligned?
	_capacity = INITIAL_CAPACITY;
}

template<class T>
inline TaskRingBuffer<T>::~TaskRingBuffer()
{
	setExiting();
	while (_popingThreadNum)
		std::this_thread::yield();
	while (_pushingThreadNum)
		std::this_thread::yield();
	//destruct all elements 
}

template<class T>
void TaskRingBuffer<T>::pushBack(T elem)
{
	TinyAssert(!_isExiting);
	if (_isExiting)
		return;
	++_pushingThreadNum;
	TinyAssert(_pushingThreadNum.load() == 1, "You can't call pushBack in two threads at the same time");
	if(isFull())
	{
		//设置block for full = true
		while (poping != 0)yeild;
		//设置更大的buffer;
		//转移当前内容
		//block for full = false
	}
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
	TinyAssert(!_isExiting);
	if (_isExiting)
		return T{};

	struct NumGuard
	{
		NumGuard() { ++_popingThreadNum; }
		~NumGuard() { --_popingThreadNum; }
	};
	NumGuard threadNumGuard;
	while (!_isExiting.load())
	{
		if (isEmpty() || blockforfull)
		{
			//mutex.lock
			//condition_variable.wait((!emtpy && !blockforfull) || _isExiting)
			if (_isExiting)
				return T{};
		}
		else
		{
			//cas get index
		}
	}
}

template<class T>
inline void TaskRingBuffer<T>::setExiting()
{
	_isExiting = true;
	_waitingForPushCondi.notify_all();
}
