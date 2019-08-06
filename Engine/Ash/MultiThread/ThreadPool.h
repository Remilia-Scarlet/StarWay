#pragma once
#include <vector>
#include <thread>
#include <shared_mutex>
#include <atomic>
#include "Ash/MultiThread/Task.h"
#include "Ash/MultiThread/TaskRingBuffer.h"
#include "Ash/RefCountPointer/RefCountPtr.h"
#include <set>
#include <functional>


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
	TaskRingBuffer<RefCountPtr<Task>> _waitingTasks;
	std::vector<std::unique_ptr<Thread> > _threads;
};
