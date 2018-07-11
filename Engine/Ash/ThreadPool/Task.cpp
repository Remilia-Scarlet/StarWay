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

void Task::addDependence(RefCountPtr<Task> dependence)
{
	if(dependence->_TaskStatus != CREATED)
	{
		TinyAssert(false, "You can only change depedence before calling ThreadPool::addTask()");
		return;
	}
	dependence->_unlock.emplace_back(this);
	_dependences.push_back(std::move(dependence));
	++_unfinishDependenceNumber;
}

void Task::waitForTaskDone()
{
	std::unique_lock<std::mutex> lock(_mutex);
	_doneCondi.wait(lock, [this]() {return _TaskStatus == FINISHE; });
}
