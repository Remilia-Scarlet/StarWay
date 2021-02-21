#pragma once
#include <memory>
#include <functional>
#include <mutex>
#include <atomic>

namespace Ash
{

	class ThreadPool;
	// Inherit this class to store your own user data
	class TaskUserData
	{
	public:
		virtual ~TaskUserData() = default;
	};

	class Task
	{
		friend class TaskGraph;
	protected:
		Task();
		Task(std::function<void(Task*)> worker);
		Task(std::function<void(Task*)> worker, std::shared_ptr<TaskUserData> userData);	// userdata can be get by getUserData. 
		Task(const Task&) = delete;
		Task(Task&&) = delete;
		virtual ~Task();

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
	protected:
		void run(std::thread::id threadId);
		void onChildFinish(Task* child);
	protected:
		std::shared_ptr<TaskUserData> _userData;
		std::function<void(Task*)> _worker;
		std::thread::id _threadId;
		std::vector<Task*> _nextTasks;
		std::vector<Task*> _childTasks;
		std::vector<Task*> _parentTasks;

		std::atomic<int> _unfinishChildrenTaskNumber{ 0 };
		ThreadPool* _threadPool{ nullptr };
		TaskGraph* _taskGraph;
	};
}