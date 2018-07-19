#include "Ash/precomp.h"
#include "ThreadPool.h"
#include <algorithm>
#include "Task.h"

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

		Task& task = *_runningTask;
		task._threadId = _thread.get_id();

		if (task._worker)
			task._worker(&task);

		task._taskStatus = Task::FINISHED;
		_threadPool.onTaskFinished(_runningTask);
		for (auto& linkTask : task._linkedTasks)
		{
			TinyAssert(linkTask.isValid());
			_threadPool.addLinkedTaskToTaskPool(linkTask);
		}

		_runningTask.reset();
		--_threadPool._unfinishedTask;
		_threadPool._waitingForUnfinishedTasksCondi.notify_all();

	}		
}

ThreadPool::ThreadPool(int maxThreadNumber, int initialTaskPoolSize)
{
	for (int i = 0; i < maxThreadNumber; ++i)
	{
		_threads.emplace_back(std::make_unique<Thread>(*this));
	}
}

ThreadPool::~ThreadPool()
{
	_isDestruction = true;
	_workerThreadWaitingForTaskPool.notify_all();
	for (auto& th : _threads)
	{
		th->_thread.join();
	}
}

void ThreadPool::addTask(RefCountPtr<Task> task)
{
	TinyAssert(task.isValid());
	TinyAssert(task->_taskStatus == Task::CREATED, "You can't add a linked task or executing task to thread pool");

	task->_taskStatus = Task::ADDED_TO_THREAD_POOL;

	addToThreadTaskPool(std::move(task));
}

void ThreadPool::waitForAllTasksFinished()
{
	Lock lock(_waitingForUnfinishedTasks);
	_waitingForUnfinishedTasksCondi.wait(lock, [this]() {return _unfinishedTask.load() == 0; });
}

RefCountPtr<Task> ThreadPool::getNextTask()
{
	Lock lock(_mutexForThreadPoolMember);
	_workerThreadWaitingForTaskPool.wait(lock, [this]() { return _waitingTasks.getSize() > 0 || _isDestruction == true; });
	if (_isDestruction)
		return nullptr;
	RefCountPtr<Task> task = std::move(_waitingTasks.front());
	_waitingTasks.popFront();
	return task;
}

void ThreadPool::onTaskFinished(RefCountPtr<Task> task)
{
	if(task->_unfinishLinkedTaskNumber == 0)
	{
		if (task->_endTask.isValid())
		{
			++task->_unfinishLinkedTaskNumber;
			addLinkEndTaskToTaskPool(std::move(task->_endTask));
		}
		else
		{
			TaskPtr parent = task->_parent.lock();
			if (parent.isValid())
			{
				int number = --parent->_unfinishLinkedTaskNumber;
				TinyAssert(number >= 0);
				if (number == 0)
					onTaskFinished(parent);
			}
			
			if (task->savedForEndTask)
			{
				Lock lock(_mutexForWaitEndTask);
				auto it = _waitEndTask.find(task);
				TinyAssert(it != _waitEndTask.end());
				_waitEndTask.erase(it);
				task->savedForEndTask = false;
			}
		}
	}
	else if(task->_endTask.isValid())
	{
		Lock lock(_mutexForWaitEndTask);
		_waitEndTask.insert(task);
		task->savedForEndTask = true;
	}
}

void ThreadPool::addLinkedTaskToTaskPool(RefCountPtr<Task> task)
{
	TinyAssert(task->_taskStatus == Task::LINKED_BY_OTHER_TASK);
	task->_taskStatus = Task::ADDED_TO_THREAD_POOL;

	addToThreadTaskPool(std::move(task));
}

void ThreadPool::addLinkEndTaskToTaskPool(RefCountPtr<Task> task)
{
	TinyAssert(task->_taskStatus == Task::AS_LINK_END_TASK);
	task->_taskStatus = Task::ADDED_TO_THREAD_POOL;

	addToThreadTaskPool(std::move(task));
}

void ThreadPool::addToThreadTaskPool(RefCountPtr<Task> task)
{
	Lock lock(_mutexForThreadPoolMember);
	TinyAssert(task->_taskStatus == Task::ADDED_TO_THREAD_POOL);
	++_unfinishedTask;
	if (_waitingTasks.getFreeCount() == 0)
		_waitingTasks.setCapacity(std::max(32, static_cast<int32_t>(_waitingTasks.getCapacity() * 1.2)));
	_waitingTasks.emplaceBackItem(std::move(task));
	_workerThreadWaitingForTaskPool.notify_one();
}
