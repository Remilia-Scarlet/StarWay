#include "Ash/precomp.h"
#include "Task.h"
#include "Ash/TinyAssert.h"
#include "ThreadPool.h"

Task::Task()
{
}

Task::Task(std::function<void(Task*)> worker)
	:_worker(std::move(worker))
{
}

Task::Task(std::function<void(Task*)> worker, std::shared_ptr<TaskUserData> userData)
	:_userData(std::move(userData))
	,_worker(std::move(worker))
{
}

Task::~Task()
{

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

void Task::addNextTask(RefCountPtr<Task> task)
{
	_nextTasks.emplace_back(std::move(task));
}

void Task::addChildTask(RefCountPtr<Task> task)
{
	_childTasks.emplace_back(std::move(task));
}

void Task::setEndTask(RefCountPtr<Task> endTask)
{
	_endTask.move(std::move(endTask));
}

void Task::blockTillFinished()
{

}

void Task::run(std::thread::id threadId)
{
	TinyAssert(_taskStatus == ADDED_TO_THREAD_POOL);
	_taskStatus = WORKING;
	if (_worker)
		_worker(this);
	for (auto& task : _nextTasks)
		_threadPool->addTask(std::move(task));
	_nextTasks.clear();
	if (_childTasks.empty())
		finish();
	else
	{
		_taskStatus = WAIT_FOR_CHILDREN_FINISH;
		for (auto& task : _childTasks)
			_threadPool->addTask(std::move(task));
		_childTasks.clear();
	}
}

void Task::finish()
{
	//status
	//parent->onchildfinish
}

void Task::onChildFinish(RefCountPtr<Task> child)
{
	//--childcount
	//if childcount == 0
		//finish

}

void Task::onAddedToThreadPool(ThreadPool* threadPool)
{
	TinyAssert(_taskStatus == Task::NEW, "You can't add a linked task or executing task to thread pool");
	_taskStatus = Task::ADDED_TO_THREAD_POOL;
	_threadPool = threadPool;
}
