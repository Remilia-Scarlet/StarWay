#include "AshCore.h"
#include "Task.h"
#include "Ash/TinyAssert.h"
#include "ThreadPool.h"

Ash::Task::Task()
{
}

Ash::Task::Task(std::function<void(Task*)> worker)
	:_worker(std::move(worker))
{
}

Ash::Task::Task(std::function<void(Task*)> worker, std::shared_ptr<TaskUserData> userData)
	:_userData(std::move(userData))
	,_worker(std::move(worker))
{
}

Ash::Task::~Task()
{

}


const std::function<void(Ash::Task*)>& Ash::Task::getWorkerFunction() const
{
	return _worker;
}

void Ash::Task::setWorkerFunction(std::function<void(Task*)> worker)
{
	_worker = std::move(worker);
}

void Ash::Task::run(std::thread::id threadId)
{
	if(_worker)
	{
		_worker(this);
	}
	for (Task* child : _childTasks)
	{
		_threadPool->pushTask(child);
	}
	onDependencyEnd();
}

void Ash::Task::addChildTask(Task* child)
{
	++_unfinishedDendency;
	_childTasks.emplace_back(child);
}

void Ash::Task::onChildFinish(Task* child)
{
	onDependencyEnd();
}

void Ash::Task::onDependencyEnd()
{
	int remain = --_unfinishedDendency;
	if (remain == 0)
	{
		for (Task* next : _nextTasks)
		{
			_threadPool->pushTask(next);
		}
		for(Task* parent : _parentTasks)
		{
			parent->onChildFinish(this);
		}
	}
}