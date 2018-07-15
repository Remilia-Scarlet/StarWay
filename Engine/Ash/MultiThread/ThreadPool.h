#pragma once
#include <vector>
#include <thread>
#include <shared_mutex>
#include <atomic>
#include "Ash/Container/RingBuffer.h"
#include "Ash/RefCountPointer/RefCountPtr.h"

typedef std::unique_lock<std::mutex> Lock;
typedef std::unique_lock<std::shared_mutex> WriteLock;
typedef std::shared_lock<std::shared_mutex> ReadLock;

class Task;
class ThreadPool
{
protected:
	struct Thread
	{
		explicit Thread(ThreadPool& threadPool, int threadId, int initialTaskPoolSize = 16);
		Thread(const Thread&) = delete;
		Thread(Thread&&) = delete;
		Thread& operator=(const Thread&) = delete;
		Thread& operator=(Thread&&) = delete;
		~Thread();

		ThreadPool& _threadPool;
		int _threadId;
		RingBuffer<RefCountPtr<Task>> _waitingTasks;
		RefCountPtr<Task> _runningTask;
		std::shared_ptr<std::mutex> _mutex{ new std::mutex };
		std::shared_ptr<std::condition_variable> _threadWaitForTaskCondi{new std::condition_variable};
		std::thread _thread{ std::bind(&Thread::run, this) };

		void run();
		void addTask(RefCountPtr<Task> task);
		int getWaitingTaskSize() const;
	};
public:
	// Can only be construct in single thread. 
	ThreadPool(int maxThreadNumber, int initialTaskPoolSize = 32);
	ThreadPool(const ThreadPool&) = delete;
	ThreadPool(ThreadPool&&) = default;
	ThreadPool& operator=(const ThreadPool&) = delete;
	ThreadPool& operator=(ThreadPool&&) = default;
	~ThreadPool();
public:
	// add a task to pool, if there are any free threads, this task can be executed immediately. Otherwise it will be put to a waiting queue.
	// one task can only be added once
	void addTask(RefCountPtr<Task> task);
	// clear the waiting queue
	void clearUnstartedTasks();
	// means waiting queue is empty and all threads finish doing work
	int doesAllTasksFinished();
	// block current thread untill waiting queue is empty and all threads finish doing work
	void waitForAllTasksFinished();
protected:
	void checkAllDependeceFinishedAndAddSelf(RefCountPtr<Task> task);
	void addToThreadTaskPool(RefCountPtr<Task> task);
	std::shared_ptr<std::atomic<bool> > _isDestruction{ new std::atomic < bool >(false) };
	std::atomic<int> _unfinishedTask = 0;
	std::mutex _waitingForUnfinishedTasks;
	std::condition_variable _waitingForUnfinishedTasksCondi;
	std::vector<std::unique_ptr<Thread> > _threads;
};
