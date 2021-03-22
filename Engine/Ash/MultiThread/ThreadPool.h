#pragma once
#include <thread>
#include <queue>
#include "boost/lockfree/queue.hpp"

#include "Ash/Misc/SingleInstance.h"
#include "Ash/MultiThread/FunctorSeq.h"
#include "Ash/MultiThread/Semaphore.h"

namespace Ash
{
	//Usage:
	//
	// auto task = []()
	// {
	//    //Do your own stuff
	// };
	// ThreadPool::instance()->dispatchTask(task);
	using ThreadPoolTask = std::function<void()>;
	
    class ThreadPool : public SingleInstance<ThreadPool>
	{
		friend class SingleInstance<ThreadPool>;
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
			std::thread _thread{ [this] { run(); } };

			void run();
		};
	public:
		void dispatchTask(ThreadPoolTask task);
	protected:
    	ThreadPool();
		~ThreadPool();
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;

		void pushTask(ThreadPoolTask* task);
		ThreadPoolTask* popTask();

		boost::lockfree::queue<ThreadPoolTask*, boost::lockfree::fixed_sized<false>> _waitingTasks{ 20 };
		std::vector<std::unique_ptr<Thread> > _threads;
		std::atomic_bool _exiting = false;

		Ash::Semaphore _semaphore;
	};

}
