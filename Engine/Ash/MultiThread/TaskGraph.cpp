#include "AshCore.h"
#include "TaskGraph.h"
#include "Ash/TinyAssert.h"

#include "Ash/MultiThread/Task.h"

Ash::Task* Ash::TaskGraph::allocTask()
{
	TinyAssert(!start);
	
	Task* task = new Task;
	task->_taskGraph = this;
	_tasks.emplace_back(task);
	return task;
}

void Ash::TaskGraph::linkNextTask(Task* before, Task* after)
{
	TinyAssert(before->_taskGraph == this && after->_taskGraph == this);
	TinyAssert(!start);
	
	before->_nextTasks.emplace_back(after);
}

void Ash::TaskGraph::linkChildTask(Task* parent, Task* child)
{
	TinyAssert(parent->_taskGraph == this && child->_taskGraph == this);
	TinyAssert(!start);
	
	parent->_childTasks.emplace_back(child);
	child->_parentTasks.emplace_back(parent);
}

void Ash::TaskGraph::setStartTask(Task* startTask)
{
	TinyAssert(!start);
	TinyAssert(startTask->_taskGraph == this);

	_startTask = startTask;
}
