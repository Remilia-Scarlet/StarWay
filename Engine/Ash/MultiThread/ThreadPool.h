#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <functional>

#include "boost/lockfree/queue.hpp"

#include "Ash/MultiThread/Task.h"

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
		~ThreadPool();

		TaskPtr dispatchTask(std::function<void(TaskPtr)> worker);
		TaskPtr dispatchTask(std::function<void(TaskPtr)> worker, std::initializer_list<TaskPtr> parents);
	protected:
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;

		void pushTask(Task* task);
		Task* popTask();
		
		boost::lockfree::queue<Task*, boost::lockfree::fixed_sized<false>> _waitingTasks;
		std::vector<std::unique_ptr<Thread> > _threads;
		std::atomic_bool _exiting = false;
	};

}
