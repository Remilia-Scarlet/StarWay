#pragma once
#include <vector>
#include <thread>
#include <atomic>
#include <functional>

#include "boost/lockfree/queue.hpp"

#include "Ash/Misc/SingleInstance.h"
#include "Ash/MultiThread/FunctorSeq.h"

namespace Ash
{
	//Usage:
	//Inherit ThreadPoolTask, and then dispatch it to threadpool
	// class MyTask : public ThreadPoolTask
	// {
	//    void onRun() override
	//    {
	//		//do something
	//    }
	// }
	// RefCountPtr<MyTask> task = Ash::MakeRefCountPtr<MyTask>();
	// ThreadPool::instance()->dispatchTask(task);
	ASH_DEFINE_PTR(ThreadPoolTask);
	class ThreadPoolTask : public RefCountObj
	{
	public:
		virtual ~ThreadPoolTask() = default;
		virtual void onRun() = 0;
	};
	
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
		void dispatchTask(ThreadPoolTaskPtr task);
	protected:
    	ThreadPool();
		~ThreadPool();
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;

		void pushTask(ThreadPoolTask* task);
		ThreadPoolTaskPtr popTask();
		
		boost::lockfree::queue<ThreadPoolTask*, boost::lockfree::fixed_sized<false>> _waitingTasks{ 20 };
		std::vector<std::unique_ptr<Thread> > _threads;
		std::atomic_bool _exiting = false;
	};

}
