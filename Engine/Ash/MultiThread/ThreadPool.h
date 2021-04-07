#pragma once
#include <thread>
#include <queue>
#include "boost/lockfree/queue.hpp"

#include "Ash/Misc/SingleInstance.h"
#include "Ash/MultiThread/FunctorSeq.h"
#include "Ash/MultiThread/Semaphore.h"

namespace Ash
{
	using FunctorEntry = void(FunctorSeq::*)(const FunctorSaving&);
	
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
		void dispatchFunctor(FunctorEntry entry, FunctorSeq* seq, FunctorSaving* functor);
	protected:
    	ThreadPool();
		~ThreadPool();
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;

        struct Task
        {
			FunctorEntry entry;
			FunctorSaving* functor;
			FunctorSeq* seq;
        };
		void pushTask(Task task);
		Task popTask();
       
		boost::lockfree::queue<Task, boost::lockfree::fixed_sized<false>> _waitingTasks{ 20 };
		std::vector<std::unique_ptr<Thread> > _threads;
		std::atomic_bool _exiting = false;

		Ash::Semaphore _semaphore;
	};

}
