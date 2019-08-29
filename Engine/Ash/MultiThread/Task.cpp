#include "AshCore.h"
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
	if(!task.isValid())
		return;
	TinyAssert(_taskStatus == TaskStatus::NEW || _taskStatus == TaskStatus::WORKING);
	task->onAddedToNextOrEnd(this);
	_nextTasks.emplace_back(std::move(task));
}

void Task::addChildTask(RefCountPtr<Task> task)
{
	if (!task.isValid())
		return;
	TinyAssert(_taskStatus == TaskStatus::NEW || _taskStatus == TaskStatus::WORKING);
	task->onAddedToChild(this);
	_childTasks.emplace_back(std::move(task));
}

void Task::addEndTask(RefCountPtr<Task> endTask)
{
	if (!endTask.isValid())
		return;
	TinyAssert(_taskStatus == TaskStatus::NEW || _taskStatus == TaskStatus::WORKING);
	endTask->onAddedToNextOrEnd(this);
	_endTasks.emplace_back(std::move(endTask));
}

void Task::addFenceTask()
{
	//use nullptr as fence task
	_childTasks.push_back(nullptr);
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
		if (!addChildrenTasksToPool())
			finish();
	}
}

void Task::finish()
{
	TinyAssert(_taskStatus == WORKING || _taskStatus == WAIT_FOR_CHILDREN_FINISH);
	_taskStatus = FINISHED;
	RefCountPtr<Task> parent = _parentOrFormerTask.lock();
	if(parent.isValid())
	{
		parent->onChildFinish(RefCountPtr<Task>(this));
	}
}

bool Task::addChildrenTasksToPool()
{
	TinyAssert(_taskStatus == TaskStatus::WAIT_FOR_CHILDREN_FINISH);
	std::vector<RefCountPtr<Task>> tmpChildVec;
	auto it = _childTasks.begin();
	for (; it != _childTasks.end(); ++it)
	{
		if (it->isValid())
			tmpChildVec.emplace_back(std::move(*it));
		else if (tmpChildVec.size())
			break;
	}
	_childTasks.erase(_childTasks.begin(), (it == _childTasks.end() ? it : it + 1));
	_unfinishChildrenTaskNumber = (int)tmpChildVec.size();
	for (auto& task : tmpChildVec)
		_threadPool->addTask(std::move(task));
	return tmpChildVec.size() > 0;
}

void Task::onChildFinish(RefCountPtr<Task> child)
{
	TinyAssert(_taskStatus == WAIT_FOR_CHILDREN_FINISH);
	--_unfinishChildrenTaskNumber;
	if (_unfinishChildrenTaskNumber == 0)
	{
		if (!addChildrenTasksToPool())
			finish();
	}
}

void Task::onAddedToThreadPool(ThreadPool* threadPool)
{
	TinyAssert(_taskStatus == Task::NEW, "You can't add a linked task or executing task to thread pool");
	_taskStatus = Task::ADDED_TO_THREAD_POOL;
	_threadPool = threadPool;
}

void Task::onAddedToChild(Task* parent)
{
	TinyAssert(_taskStatus == TaskStatus::NEW);
	TinyAssert(!_parentOrFormerTask.isValid(), "A task can only have one parent");
	_parentOrFormerTask = parent;
	_iAmChildOfOtherTask = true;
}

void Task::onAddedToNextOrEnd(Task* formerTask)
{
	TinyAssert(_taskStatus == TaskStatus::NEW);
	TinyAssert(!_parentOrFormerTask.isValid(), "A task can not be child task and next task of other tasks at the same time");
	_parentOrFormerTask = formerTask;
	_iAmChildOfOtherTask = false;
}
