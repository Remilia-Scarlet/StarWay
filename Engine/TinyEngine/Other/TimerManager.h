#pragma once
#include "TinyEngine/Other/Timer.h"

class TimerManager
{
public:
	static TimerManager* instance();
	static bool createInstance();
	static void destoryInstance();
public:
	// Only when enable == true, all timers will work.
	void setEnable(bool enable);
	bool getEnable();

	// Add a timer to timer list, it will work since now. Calling Timer.start() will call this function.
	void addTimer(const Timer& timer);

	// Try to remove a timer. If found and removed, return true, otherwise return false. Calling Timer.stop() will call this function.
	bool removeTimer(int id);

	// remove all timer
	void removeAll();

	// Try to search a timer in timer list. Only when Timer.isWorking() == true, you can get it.
	Timer* getTimer(int id);

	// Called by Engine, do not call it manually.
	void update(float dt);
protected:
	bool init();

	bool _enable;
	std::unordered_map<int,Timer> _timers;
protected:
	static TimerManager* s_instance;
	TimerManager();
	~TimerManager();
};