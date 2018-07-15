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

	// if task is running, get the id of thread which is running the task
	int getThreadID() const;

	// If you add a dependence to this task, when you add this task to threadpool, the dependence task will also be added first
	// And threadpool will ensure dependence tasks are finished when executing this task.
	void addDependence(RefCountPtr<Task> dependence);
	const std::vector<RefCountPtr<Task> >& getDependence() const;

	void linkTask(RefCountPtr<Task> task);
	void unlinkTask(RefCountPtr<Task> task);


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
	std::vector<RefCountPtr<Task> > _dependences;
	std::vector<Task*> _unlock;
	std::atomic<int> _unfinishDependenceNumber = 0;
	RefCountPtr<Task> _mySelf;
};
