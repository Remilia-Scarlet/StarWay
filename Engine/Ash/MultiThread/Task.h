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
		friend class ThreadPool;
	protected:
		Task(std::function<void(TaskPtr)> worker);
		Task(std::function<void(TaskPtr)> worker, std::initializer_list<TaskPtr> parents, std::initializer_list<TaskPtr> preposedTask);
		Task(const Task&) = delete;
		Task(Task&&) = delete;
		virtual ~Task();

		Task& operator=(const Task&) = delete;
		Task& operator=(Task&&) = delete;
	protected:
		void onRun(std::thread::id threadId);
		
		void addChildTask(Task* child);
		void onChildFinish(Task* child);
		void onDependencyEnd();
	protected:
		std::function<void(TaskPtr)> _worker;

		std::vector<TaskPtr> _nextTasks;
		std::vector<TaskPtr> _childTasks;
		std::vector<Task*> _parentTasks;

		std::atomic_int32_t _unfinishedDendency = 1;
	};
}
