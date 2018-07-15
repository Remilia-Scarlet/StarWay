#include "precomp.h"
#include "ThreadPool.h"
#include <algorithm>
#include "Task.h"

ThreadPool::Thread::Thread(ThreadPool& threadPool, int threadId, int initialTaskPoolSize)
	: _threadPool(threadPool)
	, _threadId(threadId)
	, _waitingTasks(static_cast<int32_t>(initialTaskPoolSize))
{
}

ThreadPool::Thread::~Thread()
{
}


void ThreadPool::Thread::run()
{
	while (true)
	{
		Lock lock(*_mutex.get());
		_runningTask.reset();
		_threadWaitForTaskCondi->wait(lock, [this]() { return _waitingTasks.getSize() > 0 || _threadPool._isDestruction->load(); });
		if(_threadPool._isDestruction->load())
			return;
		_runningTask = std::move(_waitingTasks.front());
		_waitingTasks.popFront();
		Task& task = *_runningTask;
		task._threadId = _threadId;
		lock.unlock();

		if (task._worker)
			task._worker(&task);

		for(auto& unlock : task._unlock)
		{
			//TinyAssert(unlock.isValid());
			RefCountPtr<Task> ptr( unlock);
			--ptr->_unfinishDependenceNumber;
			TinyAssert(ptr->_unfinishDependenceNumber >= 0);
			_threadPool.checkAllDependeceFinishedAndAddSelf(std::move(ptr));
		}
		task._TaskStatus = Task::FINISHE;
		--_threadPool._unfinishedTask;
		_threadPool._waitingForUnfinishedTasksCondi.notify_all();
		task._doneCondi.notify_all();
	}		
}

void ThreadPool::Thread::addTask(RefCountPtr<Task> task)
{
	Lock lock(*_mutex.get());
	if (_waitingTasks.getFreeCount() == 0)
		_waitingTasks.setCapacity(std::max(16, static_cast<int32_t>(_waitingTasks.getCapacity() * 1.2)));
	_waitingTasks.emplaceBackItem(std::move(task));
	lock.unlock();
	_threadWaitForTaskCondi->notify_one();
}

int ThreadPool::Thread::getWaitingTaskSize() const
{
	return static_cast<int>(_waitingTasks.getSize());
}

ThreadPool::ThreadPool(int maxThreadNumber, int initialTaskPoolSize)
{
	for (int i = 0; i < maxThreadNumber; ++i)
	{
		_threads.emplace_back(std::make_unique<Thread>(*this, i, initialTaskPoolSize));
	}
}

ThreadPool::~ThreadPool()
{
	*_isDestruction = true;
	for(auto& th : _threads)
	{
		th->_threadWaitForTaskCondi->notify_one();
		th->_thread.join();
	}
}

void ThreadPool::addTask(RefCountPtr<Task> task)
{
	TinyAssert(task.isValid());

	//ensure everytask is added only once
	Task::TaskStatus status = Task::CREATED;
	bool changeStatusResult = task->_TaskStatus.compare_exchange_strong(status, Task::ADDED_TO_THREAD_POOL);
	if (!changeStatusResult)
		return;
	TinyAssert(status == Task::CREATED);

	task->_mySelf = task;
	++_unfinishedTask;
	for (auto& depend : task->_dependences)
	{
		addTask(depend);
	}
	checkAllDependeceFinishedAndAddSelf(std::move(task));
}

void ThreadPool::waitForAllTasksFinished()
{
	Lock lock(_waitingForUnfinishedTasks);
	_waitingForUnfinishedTasksCondi.wait(lock, [this]() {return _unfinishedTask.load() == 0; });
}

void ThreadPool::checkAllDependeceFinishedAndAddSelf(RefCountPtr<Task> task)
{
	if (task->_unfinishDependenceNumber == 0)
	{
		addToThreadTaskPool(task);
		task->_mySelf = nullptr;
	}
}

void ThreadPool::addToThreadTaskPool(RefCountPtr<Task> task)
{
	Task::TaskStatus status = Task::ADDED_TO_THREAD_POOL;
	bool changeStatusResult = task->_TaskStatus.compare_exchange_strong(status, Task::SUBMITTED_TO_THREAD);
	if(!changeStatusResult)
		return;
	TinyAssert(status == Task::ADDED_TO_THREAD_POOL);

	int minWaitingTaskSize = INT_MAX;
	int minWaitingTaskIndex = 0;
	for (int i = 0; i < static_cast<int>(_threads.size()); ++i)
	{
		int currentSize = _threads[i]->getWaitingTaskSize();
		if (currentSize < minWaitingTaskSize)
		{
			minWaitingTaskIndex = i;
			minWaitingTaskSize = currentSize;
		}
	}
	_threads[minWaitingTaskIndex]->addTask(std::move(task));
}
