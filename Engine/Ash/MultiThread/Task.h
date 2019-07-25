#pragma once
#include <memory>
#include <functional>
#include <mutex>
#include <atomic>
#include "Ash/RefCountPointer/RefCountPtr.h"
#include "Ash/RefCountPointer/RefCountObj.h"
class ThreadPool;
// Inherit this class to store your own user data
class TaskUserData
{
public:
	virtual ~TaskUserData() = default;
};
TINY_DEFINE_PTR(Task);
/*
 * All methods in Task are NOT thread safe. Which means you can't modify a Task in different threads at the same time.
 */
class Task : public RefCountObj
{
	friend class ThreadPool;
public:
	Task();
	Task(std::function<void(Task*)> worker);
	Task(std::function<void(Task*)> worker, std::shared_ptr<TaskUserData> userData);	// userdata can be get by getUserData. 
	virtual ~Task();
public:
	template<class UserDataType>
	std::shared_ptr<UserDataType> getUserData() const
	{
		return std::static_pointer_cast<UserDataType>(_userData);
	}
	template<class UserDataType>
	void setUserData(std::shared_ptr<UserDataType> userData)
	{
		_userData = std::static_pointer_cast<TaskUserData>(userData);
	}

	const std::function<void(Task*)>& getWorkerFunction() const;
	void setWorkerFunction(std::function<void(Task*)> worker);

	// if task is running, get the id of thread which is running the task
	std::thread::id getThreadID() const;

	/*
	* Next tasks will be added to thread pool immediately after this worker function finished.
	*
	* For example:
	* task1.addNextTask(task2);
	* task1.addNextTask(task3);
	* threadPool.addTask(task1);
	* When the worker function of task1 is finished, task2 and task3 will both be added to thread pool immediately.
	*/
	void addNextTask(RefCountPtr<Task> task);

	/*
	 * Children tasks will be added to thread pool immediately after this worker function finished.
	 * Used together with setEndTask.
	 */
	void addChildTask(RefCountPtr<Task> task);

	/*
	* End task will be added to thread pool after all children tasks finished.
	* For example:
	* task1.addChildTask(task2);
	* task1.addChildTask(task3);
	* task1.setEndTask(task4);
	* task1.addNextTask(task5);
	* threadPool.addTask(task1);
	* When the worker function of task1 is finished, task2, task3 and task5 will be added to thread pool immediately.
	* However task4 will be added to thread pool after task2 and task3 are both finished.
	*/
	void setEndTask(RefCountPtr<Task> endTask);
protected:
	enum TaskStatus
	{
		CREATED,
		LINKED_BY_OTHER_TASK,
		AS_LINK_END_TASK,
		ADDED_TO_THREAD_POOL,
		FINISHED
	};
	std::shared_ptr<TaskUserData> _userData;
	std::function<void(Task*)> _worker;
	TaskStatus _taskStatus = CREATED;
	std::thread::id _threadId;
	std::vector<RefCountPtr<Task>> _linkedTasks;
	RefCountPtr<Task> _endTask;
	bool savedForEndTask = false;
	WeakRefPtr<Task> _parent;
	std::atomic<int> _unfinishLinkedTaskNumber{ 0 };
};
