#include "precomp.h"
#include "Task.h"
#include "TinyAssert.h"

Task::Task(std::function<void(Task*)> worker, std::shared_ptr<TaskUserData> userData)
	:_userData(std::move(userData))
	, _worker(std::move(worker))
{
}


int Task::getThreadID() const
{
	return _threadId;
}

void Task::linkTask(RefCountPtr<Task> task)
{
	if (task->_TaskStatus != CREATED)
	{
		TinyAssert(false, "You can't link an executing task!");
		return;
	}
	++task->_unfinishDependenceNumber;
	_linkedTasks.emplace_back(std::move(task));
	TinyAssert(_linkedTasks.back()->_TaskStatus == CREATED);
}

void Task::unlinkTask(const RefCountPtr<Task>& task)
{
	if (task->_TaskStatus != CREATED)
	{
		TinyAssert(false, "You can't unlink an executing task!");
		return;
	}
	auto it = std::find(_linkedTasks.begin(), _linkedTasks.end(), task);
	if (it != _linkedTasks.end())
	{
		--task->_unfinishDependenceNumber;
		_linkedTasks.erase(it);
	}
	TinyAssert(task->_TaskStatus == CREATED);
}

void Task::waitForTaskDone()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_doneCondi.wait(lock, [this]() {return _TaskStatus == FINISHE; });
}
