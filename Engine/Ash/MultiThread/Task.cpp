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
	_linkedTasks.clear();
	_endTask.reset();
}


const std::function<void(Task*)>& Task::getWorkerFunction() const
{
	return _worker;
}

void Task::setWorkerFunction(std::function<void(Task*)> worker)
{
	_worker = std::move(worker);
}

std::thread::id Task::getThreadID() const
{
	return _threadId;
}

void Task::linkTask(RefCountPtr<Task> task)
{
	TinyAssert((_taskStatus == Task::CREATED || _taskStatus == Task::LINKED_BY_OTHER_TASK || _taskStatus == Task::AS_LINK_END_TASK)
		|| (_taskStatus == Task::ADDED_TO_THREAD_POOL && _threadId == std::this_thread::get_id())
		, "You can only call linkTask either before you adding this task to thread pool, or in its worker function");
	TinyAssert(task->_taskStatus == CREATED, "You can only link a task before it is added to thread pool");
	task->_taskStatus  = Task::LINKED_BY_OTHER_TASK;
	task->_parent = TaskWeakPtr(this);
	++_unfinishLinkedTaskNumber;
	_linkedTasks.emplace_back(std::move(task));
}

void Task::unlinkTask(const RefCountPtr<Task>& task)
{
	TinyAssert(_taskStatus == Task::CREATED || _taskStatus == Task::LINKED_BY_OTHER_TASK || _taskStatus == Task::AS_LINK_END_TASK
		|| (_taskStatus == Task::ADDED_TO_THREAD_POOL && _threadId == std::this_thread::get_id())
		, "You can only call unlinkTask either before you adding this task to thread pool, or in its worker function");
	auto it = std::find(_linkedTasks.begin(), _linkedTasks.end(), task);
	TinyAssert(it != _linkedTasks.end(), "You can't unlink a task that haven't been linked to you");
	TinyAssert(task->_taskStatus == Task::LINKED_BY_OTHER_TASK, "You can't unlink a executing task.");
	task->_taskStatus = Task::CREATED;
	task->_parent = nullptr;
	--_unfinishLinkedTaskNumber;
	_linkedTasks.erase(it);
}

void Task::linkEndTask(RefCountPtr<Task> task)
{
	TinyAssert(task.isValid());
	TinyAssert((_taskStatus == Task::CREATED || _taskStatus == Task::LINKED_BY_OTHER_TASK || _taskStatus == Task::AS_LINK_END_TASK)
		|| (_taskStatus == Task::ADDED_TO_THREAD_POOL && _threadId == std::this_thread::get_id())
		, "You can only call linkEndTask either before you adding this task to thread pool, or in its worker function");
	TinyAssert(!_endTask.isValid(), "There is already an end task");
	TinyAssert(task->_taskStatus == Task::CREATED, "You can't only linkEndTask to a new task.");
	task->_taskStatus = Task::AS_LINK_END_TASK;
	task->_parent = TaskWeakPtr(this);
	_endTask = task;
}

void Task::clearEndTask()
{
	TinyAssert((_taskStatus == Task::CREATED || _taskStatus == Task::LINKED_BY_OTHER_TASK || _taskStatus == Task::AS_LINK_END_TASK)
		|| (_taskStatus == Task::ADDED_TO_THREAD_POOL && _threadId == std::this_thread::get_id())
		, "You can only call clearEndTask either before you adding this task to thread pool, or in its worker function");
	_endTask->_taskStatus = Task::CREATED;
	_endTask->_parent = nullptr;
	_endTask.reset();
}
