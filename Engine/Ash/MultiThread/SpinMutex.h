#pragma once

#include <atomic>
#include <thread>

class SpinMutex
{
public:
	SpinMutex() = default;
	SpinMutex(const SpinMutex&) = delete;
	SpinMutex operator=(const SpinMutex&) = delete;
	void lock()
	{
		while (_flag.test_and_set(std::memory_order_acquire))std::this_thread::yield();
	}
	bool try_lock()
	{
		return !_flag.test_and_set(std::memory_order_acquire);
	}
	void unlock()
	{
		_flag.clear(std::memory_order_release);
	}
protected:
	std::atomic_flag _flag = ATOMIC_FLAG_INIT;
};