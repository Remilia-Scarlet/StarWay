#include "ThreadPool.h"

Ash::ThreadPool::Thread::Thread(ThreadPool& threadPool)
	: _threadPool(threadPool)
{
}

Ash::ThreadPool::Thread::~Thread()
{
}


void Ash::ThreadPool::Thread::run()
{
	while (!_threadPool._exiting)
	{
		ThreadPoolTask* runningTask = _threadPool.popTask();
		if(!runningTask)
		{
			continue;
		}

		(*runningTask)();
        //new in ThreadPool::dispatchTask. boost::lockfree::queue need trivial type
		delete runningTask;
	}		
}

Ash::ThreadPool::ThreadPool()
{
	auto threadNumber = std::thread::hardware_concurrency();
	if (threadNumber == 0)
		threadNumber = 1;
	for (int i = 0; i < static_cast<int>(threadNumber); ++i)
	{
		_threads.emplace_back(std::make_unique<Thread>(*this));
	}
}

Ash::ThreadPool::~ThreadPool()
{
	_exiting = true;
	_semaphore.release(99999);
	for (auto& th : _threads)
	{
		th->_thread.join();
	}
}

void Ash::ThreadPool::dispatchTask(ThreadPoolTask task)
{
	// boost::lockfree::queue need trivial type, so we have to make a heap copy here. Deleting in ThreadPool::Thread::run
	ThreadPoolTask* copyInHeap = new ThreadPoolTask(std::move(task));
	pushTask(copyInHeap);
}

void Ash::ThreadPool::pushTask(ThreadPoolTask* task)
{
	_waitingTasks.push(task);
	_semaphore.release();
}

Ash::ThreadPoolTask* Ash::ThreadPool::popTask()
{
	_semaphore.acquire();
	ThreadPoolTask* task = nullptr;
	if (_waitingTasks.pop(task))
	{
		TinyAssert(task);
		return task;
	}
	return nullptr;
}
