#pragma once
#include <memory>
#include <functional>
#include <mutex>
#include <atomic>

#include "Ash/RefCountPointer/RefCountPtr.h"
#include "Ash/RefCountPointer/RefCountObj.h"

namespace Ash
{
	TINY_DEFINE_PTR(Task);
	class Task : public RefCountObj
	{
		friend class TaskGraph;
		friend class ThreadPool;
	protected:
		Task(std::function<void(Task*)> worker);
		Task(std::function<void(Task*)> worker, std::initializer_list<TaskPtr> parents);
		Task(const Task&) = delete;
		Task(Task&&) = delete;
		virtual ~Task();

		Task& operator=(const Task&) = delete;
		Task& operator=(Task&&) = delete;
	protected:
		void run(std::thread::id threadId);
		
		void addChildTask(Task* child);
		void onChildFinish(Task* child);
		void onDependencyEnd();
	protected:
		std::function<void(Task*)> _worker;

		std::vector<Task*> _nextTasks;
		std::vector<Task*> _childTasks;
		std::vector<Task*> _parentTasks;

		ThreadPool* _threadPool{ nullptr };
		TaskGraph* _taskGraph;
		std::atomic_int32_t _unfinishedDendency = 1;
	};
}
