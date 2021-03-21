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
			std::unique_lock<std::mutex> lock(_threadPool._threadWaitingCondiMu);
			_threadPool._threadWaitingCondi.wait(lock);
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
	_threadWaitingCondi.notify_all();
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
#if USE_LOCKFREE_CONTAINER_FOR_THREADPOOL
	_waitingTasks.push(task);
#else
	std::unique_lock<std::mutex> taskMutexLock(_taskMutex);
	_waitingTasks.push(task);
	taskMutexLock.unlock();
#endif
	std::unique_lock<std::mutex> lock(_threadWaitingCondiMu);
	_threadWaitingCondi.notify_one();
}

Ash::ThreadPoolTaskPtr Ash::ThreadPool::popTask()
{
	ThreadPoolTask* task = nullptr;
#if USE_LOCKFREE_CONTAINER_FOR_THREADPOOL
	if (_waitingTasks.pop(task))
	{
#else
	std::unique_lock<std::mutex> taskMutexLock(_taskMutex);
	if(!_waitingTasks.empty())
	{
		task = _waitingTasks.front();
		_waitingTasks.pop();
		taskMutexLock.unlock();
#endif

		TinyAssert(task);
		ThreadPoolTaskPtr ptr{ task };
		task->releaseRef();
		return ptr;
	}
	return nullptr;
}
