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
		ThreadPoolTaskPtr runningTask = _threadPool.popTask();
		if(!runningTask.get())
		{
			std::this_thread::yield();
			continue;
		}

		runningTask->onRun();
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
	for (auto& th : _threads)
	{
		th->_thread.join();
	}
}

void Ash::ThreadPool::dispatchTask(ThreadPoolTaskPtr task)
{
	pushTask(task.get());
}

void Ash::ThreadPool::pushTask(ThreadPoolTask* task)
{
	task->addRef();
	_waitingTasks.push(task);
}

Ash::ThreadPoolTaskPtr Ash::ThreadPool::popTask()
{
	ThreadPoolTask* task = nullptr;
	if (_waitingTasks.pop(task))
	{
		TinyAssert(task);
		ThreadPoolTaskPtr ptr{ task };
		task->releaseRef();
		return ptr;
	}
	return nullptr;
}
