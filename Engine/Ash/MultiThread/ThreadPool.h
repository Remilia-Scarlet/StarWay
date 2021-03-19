#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <functional>

#include "boost/lockfree/queue.hpp"

#include "Ash/MultiThread/Task.h"
#include "Ash/Misc/SingleInstance.h"

namespace Ash
{
    class Functor;

    class ThreadPool : public SingleInstance<ThreadPool>
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
        static 
		ThreadPool(int threadNumber);
		~ThreadPool();

		TaskPtr dispatchFunctor(Functor&& functor);
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
