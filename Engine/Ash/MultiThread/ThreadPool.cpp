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
		Task task = _threadPool.popTask();
		if(!task.entry)
		{
			continue;
		}

		(*task.entry)(*task.functor, task.seq);
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

void Ash::ThreadPool::dispatchFunctor(FunctorEntry entry, Functor* functor, FunctorSeq* seq)
{
	pushTask(Task{ entry, functor, seq });
}

void Ash::ThreadPool::pushTask(Task task)
{
	_waitingTasks.push(task);
	_semaphore.release();
}

Ash::ThreadPool::Task Ash::ThreadPool::popTask()
{
	_semaphore.acquire();
	Task task{ nullptr, nullptr, nullptr };
	if (_waitingTasks.pop(task))
	{
		return task;
	}
	return { nullptr, nullptr, nullptr };
}
