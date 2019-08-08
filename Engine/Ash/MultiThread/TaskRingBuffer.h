#pragma once
#include <thread>
#include <atomic>
#include <functional>
#include <optional>
#include "Ash/TinyAssert.h"


class Task;
template<class T>
class TaskRingBuffer
{
protected:
	const static int INITIAL_CAPACITY = 2048;
public:
	TaskRingBuffer(int32_t capacity = INITIAL_CAPACITY);
	virtual ~TaskRingBuffer();

	void pushBack(T elem);
	std::optional<T> popFront();
	void setExiting();
protected:
	struct NumGuard
	{
		NumGuard(std::atomic<int32_t>& num) :_num(num) { ++_num; }
		~NumGuard() { --_num; }
		std::atomic<int32_t>& _num;
	};
	enum class Status : uint8_t
	{
		UNINITIALIZED,
		WRITING,
		FINISH_WRITING,
		READING
	};
protected:
	std::atomic<int32_t> _popingThreadNum{ 0 };
	std::atomic<int32_t> _popingWaitingThreadNum{ 0 };
	std::atomic<int32_t> _pushingThreadNum{ 0 };
	std::atomic<bool> _isExiting{ false };
	std::atomic<bool> _blockForFull{ false };
	std::mutex _waitingForPushMtx;
	std::condition_variable _waitingForPushCondi;
	T* _data = nullptr;
	std::vector<std::atomic<Status>> _dataFlag;
	int32_t _capacity = 0;
	std::atomic<int32_t> _front{ 0 };
	std::atomic<int32_t> _back{ 0 };

	bool isFull() const;
	bool isEmpty() const;
	void increaseCapacity();


};


#include "TaskRingBuffer.inl"