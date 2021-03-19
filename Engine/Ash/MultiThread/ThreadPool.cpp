#include "Ash/AshCore.h"
#include "ThreadPool.h"

#include <algorithm>

#include "Ash/TinyAssert.h"
#include "Ash/MultiThread/Task.h"

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
		_runningTask = _threadPool.popTask();
		if(!_runningTask)
		{
			std::this_thread::yield();
			continue;
		}

		_runningTask->onRun(_thread.get_id());
	}		
}

Ash::ThreadPool::ThreadPool(int threadNumber)
{
	for (int i = 0; i < threadNumber; ++i)
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

Ash::TaskPtr Ash::ThreadPool::dispatchTask(std::function<void(TaskPtr)> worker)
{
	TaskPtr task = TaskPtr{ new Task(std::move(worker)) };
	pushTask(task.get());
	return task;
}

Ash::TaskPtr Ash::ThreadPool::dispatchTask(std::function<void(TaskPtr)> worker, std::initializer_list<TaskPtr> parents,
    std::initializer_list<TaskPtr> preposedTask)
{
	TaskPtr task = TaskPtr{ new Task(std::move(worker), parents, preposedTask) };
	pushTask(task.get());
	return task;
}

void Ash::ThreadPool::pushTask(Task* task)
{
	task->addRef();
	_waitingTasks.push(task);
}
