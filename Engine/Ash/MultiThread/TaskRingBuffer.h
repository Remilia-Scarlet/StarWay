#pragma once
#include <thread>
#include <atomic>
#include <functional>
#include <optional>
#include "Ash/TinyAssert.h"

class Task;

/*
 * Multi-thread writable and multi-thread readable circle buffer
 * RESIZE_ON_FULL: If true, when buffer is full, it will resize automatically. Otherwise, it will block pushBack method till an empty slot occurs.
 */
template<class T, bool RESIZE_ON_FULL = true>
class TaskRingBuffer
{
public:
	TaskRingBuffer(int32_t capacity = 32);
	virtual ~TaskRingBuffer();

	// Push an element to a circle queue. If buffer is full, it will block current thread to either resize or just wait for an empty slot to occur based on RESIZE_ON_FULL
	void pushBack(T elem);
	// Pop an element from the circle queue. When buffer is empty, it will block current thread till an available element occurs. 
	std::optional<T> popFront();
	// Set exiting flag, All waiting popping thread will exit with an invalid optional return value. Will automatically be called when constructing.
	void setExiting();
protected:
	template<class NumType>
	struct NumGuard
	{
		NumGuard(NumType& num) :_num(num) { activeGuard(); }
		NumGuard(NumType& num, bool deferred) :_num(num) { if (!deferred) activeGuard(); }
		~NumGuard() { deactiveGuard(); }
		void activeGuard() { if (!_needClear) { _needClear = true; ++_num; } }
		void deactiveGuard() { if (_needClear) { _needClear = false;  --_num; } }
		NumType& _num;
		bool _needClear{ false };
	};
protected:
	std::atomic<int32_t> _popingThreadNum{ 0 };
	std::atomic<int32_t> _popingWaitingThreadNum{ 0 };
	std::atomic<int32_t> _pushingThreadNum{ 0 };
	std::atomic<int32_t> _workingPushingThreadNum{ 0 };
	std::atomic<int32_t> _workingPopThreadNum{ 0 };
	std::atomic<bool> _isExiting{ false };
	std::atomic<bool> _blockForFull{ false };
	std::mutex _waitingForPushMtx;
	std::mutex _pushMutex;
	std::mutex _popMutex;
	std::condition_variable _waitingForPushCondi;
	T* _data = nullptr;
	std::vector<std::mutex> _dataFlag;
	int32_t _capacity = 0;
	std::atomic<int32_t> _front{ 0 };
	std::atomic<int32_t> _back{ 0 };

	bool isFull() const;
	bool isEmpty() const;
	void increaseCapacity();
};


#include "TaskRingBuffer.inl"