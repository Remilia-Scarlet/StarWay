#include "AshCore.h"
#include "ThreadPool.h"
#include <algorithm>


#include "Ash/TinyAssert.h"
#include "Ash/MultiThread/Task.h"
#include "Ash/MultiThread/TaskGraph.h"

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

		_runningTask->run(_thread.get_id());
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

void Ash::ThreadPool::pushTask(Task* task)
{
	bool ret = _waitingTasks.push(task);
	TinyAssert(ret);
}

Ash::Task* Ash::ThreadPool::popTask()
{
	Task* task;
	bool ret = _waitingTasks.pop(task);
	return ret ? task : nullptr;
}

void Ash::ThreadPool::runTaskGraph(TaskGraph* taskGraph)
{
	if(!taskGraph->getStartTask())
	{
		TinyAssert(false, "No start task is set");
		return;
	}

	//compile task graph

	//check loop

	//push task
	pushTask(taskGraph->getStartTask());
}
