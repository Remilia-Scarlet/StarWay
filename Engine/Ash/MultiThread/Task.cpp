#include "Task.h"
#include "Ash/TinyAssert.h"
#include "ThreadPool.h"

Ash::Task::Task(std::function<void(TaskPtr)> worker)
	:_worker(std::move(worker))
{
}

Ash::Task::Task(std::function<void(TaskPtr)> worker, std::initializer_list<TaskPtr> parents,
    std::initializer_list<TaskPtr> preposedTask)
    :_worker(std::move(worker))
{
    for(auto& parentPtr : parents)
    {
		parentPtr->addChildTask(this);
    }
    for(auto& prepoPtr : preposedTask)
    {
		prepoPtr->_nextTasks.emplace_back(this);
    }
}

Ash::Task::~Task()
{

}

void Ash::Task::onRun(std::thread::id threadId)
{
	if(_worker)
	{
		_worker(TaskPtr{ this });
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
	child->_parentTasks.emplace_back(this);
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