#pragma once
#include <vector>
#include <thread>
#include <shared_mutex>
#include <atomic>
#include "Ash/Container/RingBuffer.h"
#include "Ash/RefCountPointer/RefCountPtr.h"
#include <set>
#include <functional>

typedef std::unique_lock<std::mutex> Lock;
typedef std::unique_lock<std::shared_mutex> WriteLock;
typedef std::shared_lock<std::shared_mutex> ReadLock;

class Task;
class ThreadPool
{
protected:
	struct Thread
	{
		explicit Thread(ThreadPool& threadPool);
		Thread(const Thread&) = delete;
		Thread(Thread&&) = delete;
		Thread& operator=(const Thread&) = delete;
		Thread& operator=(Thread&&) = delete;
		~Thread();

		ThreadPool& _threadPool;		
		RefCountPtr<Task> _runningTask;
		std::thread _thread{ std::bind(&Thread::run, this) };

		void run();
	};
public:
	ThreadPool(int threadNumber);
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = delete;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = delete;
	~ThreadPool();
public:
	// add a task to pool, if there are any free threads, this task can be executed immediately. Otherwise it will be put into a waiting queue.
	// one task can only be added once
	void addTask(RefCountPtr<Task> task);
protected:
	RefCountPtr<Task> getNextTask();
	void onTaskFinished(RefCountPtr<Task> task);
	void addLinkedTaskToTaskPool(RefCountPtr<Task> task);
	void addLinkEndTaskToTaskPool(RefCountPtr<Task> task);
	void addToThreadTaskPool(RefCountPtr<Task> task);
	std::mutex _mutexForThreadPoolMember;
	std::condition_variable _workerThreadWaitingForTaskPool;
	RingBuffer<RefCountPtr<Task>> _waitingTasks;

	std::mutex _mutexForWaitEndTask;
	std::set<RefCountPtr<Task>> _waitEndTask;

	std::atomic<bool> _isDestruction{ false };
	std::atomic<int> _unfinishedTask = 0;
	
	std::mutex _waitingForUnfinishedTasks;
	std::condition_variable _waitingForUnfinishedTasksCondi;

	std::vector<std::unique_ptr<Thread> > _threads;
};
