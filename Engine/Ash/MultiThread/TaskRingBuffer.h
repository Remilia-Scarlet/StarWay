#pragma once
#include <thread>
#include <atomic>
#include <functional>

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
	bool isFull();
	bool isEmpty();
protected:
	std::atomic<int32_t> _popingThreadNum{ 0 };
	std::atomic<int32_t> _pushingThreadNum{ 0 };
	std::atomic<bool> _isExiting{ false };
	std::mutex _waitingForPushMtx;
	std::condition_variable _waitingForPushCondi;
	T* _data = nullptr;
	int32_t _capacity = 0;
	std::atomic<int32_t> _front;
	std::atomic<int32_t> _back;
};


#include "TaskRingBuffer.inl"