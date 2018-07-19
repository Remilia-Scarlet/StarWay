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
TINY_DEFINE_PTR(Task);
/*
 * All methods in Task are NOT thread safe. Which means you can't modify a Task in different threads at the same time.
 */
class Task : public RefCountObj
{
public:
	friend class ThreadPool;
	// userdata can be get by getUserData. 
	Task(std::function<void(Task*)> worker = [](Task*) {}, std::shared_ptr<TaskUserData> userData = nullptr);
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
	* Set the task after this task.
	* You can link this task to multi tasks. When this task is finished, those tasks will be execute.
	*
	* For example:
	* task1.linkTask(task2);
	* task1.linkTask(task3);
	* threadPool.addTask(task1);
	* When task1 is finished, task2 and task3 will both be executed.
	*/
	void linkTask(RefCountPtr<Task> task);
	void unlinkTask(const RefCountPtr<Task>& task);

	/*
	 * When all linked task are finished, the end task will be executed.
	 * For example:
	 * 
	 * task1.linkTask(task2);
	 * task1.linkTask(task3);
	 * task2.linkTask(task7);
	 * task1.linkEndTask(task4);
	 * task4.linkTask(task5);
	 * task4.linkEndTask(task6);
	 * threadPool.addTask(task1);
	 * 
	 * Firstly task1 will be added to pool. 
	 * When task1 is finished, task2 and task3 will be added.
	 * When task2 is finished, task7 will be added. 
	 * When task7 and task3 are both finished, task4 will be added.
	 * When task4 finished, task5 will be added.
	 * When task5 finished, task6 will be added.
	 */
	void linkEndTask(RefCountPtr<Task> task);
	void clearEndTask();
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
