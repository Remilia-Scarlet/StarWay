#pragma once
#include <mutex>
#include <condition_variable>

namespace Ash
{
    class Semaphore
    {
    public:
		void release(int count = 1)
		{
			TinyAssert(count >= 1);
			std::unique_lock<std::mutex> lock(_mutex);
			_count += count;
			if (count == 1)
				_condi.notify_one();
			else
				_condi.notify_all();
		}
		void acquire()
		{
			std::unique_lock<std::mutex> lock(_mutex);
			_condi.wait(lock, [this]() {return _count > 0; });
			--_count;
		}
    private:
		int _count = 0;
		std::mutex _mutex;
		std::condition_variable _condi;
    };

}
