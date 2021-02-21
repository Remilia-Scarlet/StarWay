#pragma once
#include <vector>
#include <thread>
#include <shared_mutex>
#include <atomic>
#include <functional>

#include "boost/lockfree/queue.hpp"

namespace Ash
{
	class TaskGraph;
	class Task;

	class ThreadPool
	{
		friend class Task;
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
			Task* _runningTask = nullptr;
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
		void runTaskGraph(TaskGraph* taskGraph);
	protected:
		void pushTask(Task* task);
		Task* popTask();
		
		boost::lockfree::queue<Task*, boost::lockfree::fixed_sized<false>> _waitingTasks;
		std::vector<std::unique_ptr<Thread> > _threads;
		std::atomic_bool _exiting = false;
	};

}
