#pragma once
#include <vector>

namespace Ash
{
	class Task;

	class TaskGraph
	{
	public:
		Task* allocTask();
		void linkNextTask(Task* before, Task* after);
		void linkChildTask(Task* parent, Task* child);

		void setStartTask(Task* startTask);
		Task* getStartTask() const {return _startTask;}
	protected:
		std::vector<Task*> _tasks;
		Task* _startTask = nullptr;
	};
}
