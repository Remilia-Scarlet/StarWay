#include "Ash/precomp.h"
#include "ThreadPool.h"
#include <algorithm>

ThreadPool::Thread::Thread(ThreadPool& threadPool)
	: _threadPool(threadPool)
{
}

ThreadPool::Thread::~Thread()
{
}


void ThreadPool::Thread::run()
{
	while (true)
	{
		_runningTask = _threadPool.getNextTask();
		if(!_runningTask.isValid())
			return;

		_runningTask->run(_thread.get_id());
		_runningTask.reset();
	}		
}

ThreadPool::ThreadPool(int threadNumber)
{
	for (int i = 0; i < threadNumber; ++i)
	{
		_threads.emplace_back(std::make_unique<Thread>(*this));
	}
}

ThreadPool::~ThreadPool()
{
	_waitingTasks.setExiting();
	for (auto& th : _threads)
	{
		th->_thread.join();
	}
}

void ThreadPool::addTask(RefCountPtr<Task> task)
{
	TinyAssert(task.isValid());
	task->onAddedToThreadPool(this);
	_waitingTasks.pushBack(std::move(task));
}

RefCountPtr<Task> ThreadPool::getNextTask()
{
	std::optional<RefCountPtr<Task>> task = _waitingTasks.popFront();
	return task ? *task : nullptr;
}