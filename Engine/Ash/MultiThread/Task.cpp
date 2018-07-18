#include "Ash/precomp.h"
#include "Task.h"
#include "Ash/TinyAssert.h"

Task::Task(std::function<void(Task*)> worker, std::shared_ptr<TaskUserData> userData)
	:_userData(std::move(userData))
	, _worker(std::move(worker))
{
}

Task::~Task()
{
	if(_TaskStatus != FINISHE)
	{
		for(auto& task: _linkedTasks)
		{
			--task->_unfinishDependenceNumber;
		}
	}
	_linkedTasks.clear();
}


int Task::getThreadID() const
{
	return _threadId;
}

void Task::linkTask(RefCountPtr<Task> task)
{
	if (task->_TaskStatus != Task::CREATED && task->_TaskStatus != Task::LINKED_BY_OTHER_TASK)
	{
		TinyAssert(false, "You can't link an executing task!");
		return;
	}
	task->_TaskStatus = Task::LINKED_BY_OTHER_TASK;
	++task->_unfinishDependenceNumber;
	_linkedTasks.emplace_back(std::move(task));
}

void Task::unlinkTask(const RefCountPtr<Task>& task)
{
	if (task->_TaskStatus != CREATED && task->_TaskStatus != Task::LINKED_BY_OTHER_TASK)
	{
		TinyAssert(false, "You can't unlink an executing task!");
		return;
	}
	auto it = std::find(_linkedTasks.begin(), _linkedTasks.end(), task);
	if (it != _linkedTasks.end())
	{
		--task->_unfinishDependenceNumber;
		if(task->_unfinishDependenceNumber == 0)
			task->_TaskStatus = Task::CREATED;
		_linkedTasks.erase(it);
	}
}

void Task::waitForTaskDone()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_doneCondi.wait(lock, [this]() {return _TaskStatus == FINISHE; });
}
