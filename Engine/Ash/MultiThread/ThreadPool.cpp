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
		task.run(_thread.get_id());
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
	task->onAddedToThreadPool(this);
	_waitingTasks.pushBack(std::move(task));
	//notify_one
}

RefCountPtr<Task> ThreadPool::getNextTask()
{
	RefCountPtr<Task> task = _waitingTasks.popFront();
	return task;

//Single circlebuf
//Put:
	//circlebuf.push_front
		//assert(front+1 is invalid)
		//¹¹½¨ÔÚfront+1
		//front++
		//notify_one

//Get:
	//RefCountPtr<Task> task= circlebuf.popback
		//int myindex = back.load
		//if circlebuf not empty
			//myindex++
			//cas myindex
		//else
			//mutex.lock
			//condition_variable.wait





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
