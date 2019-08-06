#pragma once
#include <thread>
#include <atomic>
#include <functional>
#include "Ash/TinyAssert.h"


class Task;
template<class T>
class TaskRingBuffer
{
public:
	TaskRingBuffer();
	virtual ~TaskRingBuffer();
public:
	void pushBack(T elem);
	T popFront();
	void setExiting();
protected:
	std::atomic<int32_t> _popingThreadNum{ 0 };
	std::atomic<int32_t> _popingWaitingThreadNum{ 0 };
	std::atomic<int32_t> _pushingThreadNum{ 0 };
	std::atomic<bool> _isExiting{ false };
	std::atomic<bool> _blockForFull{ false };
	std::mutex _waitingForPushMtx;
	std::condition_variable _waitingForPushCondi;
	T* _data = nullptr;
	int32_t _capacity = 0;
	std::atomic<int32_t> _front{ 0 };
	std::atomic<int32_t> _back{ 0 };

	bool isFull() const;
	bool isEmpty() const;
	void increaseCapacity();
protected:
	const static int INITIAL_CAPACITY = 2048;
	const static int KEEP_CAPACITY = 1024;
	static_assert(INITIAL_CAPACITY > KEEP_CAPACITY && KEEP_CAPACITY >= 1);
};


#include "TaskRingBuffer.inl"