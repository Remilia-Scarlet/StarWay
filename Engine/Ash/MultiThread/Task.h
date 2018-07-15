#pragma once
#include <memory>
#include <functional>
#include <mutex>
#include <atomic>
#include "Ash/RefCountPointer/RefCountPtr.h"
#include "Ash/RefCountPointer/RefCountObj.h"
class ThreadPool;
// Inherit this class to store your own user data
struct TaskUserData
{
	virtual ~TaskUserData() = default;
};

class Task : public RefCountObj
{
	friend class ThreadPool;
public:
	// userdata can be get by getUserData. 
	Task(std::function<void(Task*)> worker = [](Task*) {} , std::shared_ptr<TaskUserData> userData = nullptr);
	virtual ~Task() = default;
public:
	template<class UserDataType>
	std::shared_ptr<UserDataType> getUserData() const { return std::static_pointer_cast<UserDataType>(_userData); }
	template<class UserDataType>
	void setUserData(std::shared_ptr<UserDataType> userData) { _userData = std::static_pointer_cast<TaskUserData>(userData); }

	const std::function<void(Task*)>& getWorkerFunction() const { return _worker; }
	void setWorkerFunction(std::function<void(Task*)> worker) { _worker = std::move(worker); }

	// if task is running, get the id of thread which is running the task
	int getThreadID() const;

	/*
	* Set the task after this task.
	* You can link this task to multi tasks. When this task is finished, those tasks will be execute.
	* If there are two or more tasks link to this task, only when they all finished, this task will be executed.
	*
	* For example:
	* task1.linkTask(task2);
	* task1.linkTask(task3);
	* threadPool.addTask(task1);
	* When task1 is finished, task2 and task3 will both be executed.
	* 
	* Another example:
	* task1.linkTask(task3);
	* task2.linkTask(task3);
	* threadPool.addTask(task1);
	* threadPool.addTask(task2);
	* When task1 and task2 both finished, task3 will be executed.
	*/
	void linkTask(RefCountPtr<Task> task);
	void unlinkTask(const RefCountPtr<Task>& task);

	//You can't call this in task worker thread.
	void waitForTaskDone();
protected:
	enum TaskStatus
	{
		CREATED,
		ADDED_TO_THREAD_POOL,
		SUBMITTED_TO_THREAD,
		FINISHE
	};
	std::shared_ptr<TaskUserData> _userData;
	std::function<void(Task*)> _worker;
	std::atomic<TaskStatus> _TaskStatus = CREATED;
	std::mutex _mutex;
	std::condition_variable _doneCondi;
	std::atomic<int> _threadId = -1;
	std::vector<RefCountPtr<Task>> _linkedTasks;
	std::atomic<int> _unfinishDependenceNumber{ 0 };
};
