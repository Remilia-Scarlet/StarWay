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
enum class TaskStatus
{
	NEW,
	ADDED_TO_THREAD_POOL,		//Already added to thread pool and wait for executing
	WORKING,					//Executing
	WAIT_FOR_CHILDREN_FINISH,	//The worker function of this task is finished, waiting for children tasks finishing
	FINISHED					//All children tasks are finished
};
TINY_DEFINE_PTR(Task);
class Task : public RefCountObj
{
	friend class ThreadPool;
public:
	Task();
	Task(std::function<void(Task*)> worker);
	Task(std::function<void(Task*)> worker, std::shared_ptr<TaskUserData> userData);	// userdata can be get by getUserData. 
	Task(const Task&) = delete;
	Task(Task&&) = delete;
	virtual ~Task();
public:
	Task& operator=(const Task&) = delete;
	Task& operator=(Task&&) = delete;
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
	* Next tasks will be added to thread pool immediately after the worker function finished.
	*
	* For example:
	* task1.addNextTask(task2);
	* task1.addNextTask(task3);
	* threadPool.addTask(task1);
	* When the worker function of task1 is finished, task2 and task3 will both be added to thread pool immediately.
	*/
	void addNextTask(RefCountPtr<Task> task);

	/*
	 * Children tasks will be added to thread pool immediately after the worker function finished.
	 * Used together with addEndTask.
	 */
	void addChildTask(RefCountPtr<Task> task);

	/*
	* End task will be added to thread pool after all children tasks finished.
	* For example:
	* task1.addChildTask(task2);
	* task1.addChildTask(task3);
	* task1.addEndTask(task4);
	* task1.addNextTask(task5);
	* threadPool.addTask(task1);
	* When the worker function of task1 is finished, task2, task3 and task5 will be added to thread pool immediately.
	* However task4 will be added to thread pool after task2 and task3 are both finished.
	* You can use addFenceTask synchronize the tasks.
	*/
	void addEndTask(RefCountPtr<Task> endTask);

	/*
	 * Add a fence task to children tasks.
	 * All children tasks that before this task will be finished, and then remaining tasks will be added.
	 * For example:
	 * task1.addChildTask(task2);
	 * task1.addChildTask(task3);
	 * task1.addFenceTask();
	 * task1.addChildTask(task4);
	 * task1.addFenceTask();
	 * task1.addChildTask(task5);
	 * task1.addChildTask(task6);
	 * task1.addChildTask(task7);
	 * task1.addEndTask(task8);
	 * threadPool.addTask(task1);
	 * When the worker function of task1 is finished, task2, task3 will be added to thread pool immediately.
	 * After both of task2 and task3 are finished, task4 will be added to thread pool.
	 * After task4 is finished, task5, task6 and task7 will be added to thread pool
	 * After task5-7 are finished, task8 will be added to thread pool.
	 */
	void addFenceTask();

	TaskStatus getTaskStatus();
protected:
	void run(std::thread::id threadId);
	void finish();
	bool addChildrenTasksToPool();
	void onChildFinish(RefCountPtr<Task> child);
	void onAddedToThreadPool(ThreadPool* threadPool);
	void onAddedToChild(Task* parent);
	void onAddedToNextOrEnd(Task* formerTask);
protected:
	std::shared_ptr<TaskUserData> _userData;
	std::function<void(Task*)> _worker;
	std::atomic<TaskStatus> _taskStatus = TaskStatus::NEW;
	std::thread::id _threadId;
	std::vector<RefCountPtr<Task>> _nextTasks;
	std::vector<RefCountPtr<Task>> _childTasks;
	std::vector<RefCountPtr<Task>> _endTasks;
	bool _iAmChildOfOtherTask = false;
	WeakRefPtr<Task> _parentOrFormerTask; //If _iAmChildOfOtherTask is false, _parentOrFormerTask is former task. Otherwise it's parent task
	std::atomic<int> _unfinishChildrenTaskNumber{ 0 };
	ThreadPool* _threadPool{ nullptr };
};