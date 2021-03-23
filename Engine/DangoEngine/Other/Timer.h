#pragma once
#include <functional>
#include <memory>

/*
Usage:

Timer timer = Timer(1,-1,someFunction);
timer.start();

Now the somefunction() will be called every 1 second.


Another example:
Print Fibonacii sequence item 1-10 every 1 second.
void test()
{
	std::function<void(Timer*)> fibonacii = [](Timer* timer) {
		int* array = (int*)timer->getUserData();
		int value = array[0] + array[1];
		array[0] = array[1];
		array[1] = value;
		DebugString("%d\n", value);
	};

	Timer timer = Timer(1, 20, fibonacii);

	timer.onStart() = [](Timer* timer) {
		if (timer->getUserData() != nullptr)
			delete timer->getUserData();
		int* array = new int[2];
		array[0] = array[1] = 1;
		timer->setUserData(array);
	};

	timer.onStop() = [](Timer* timer) {
		if (timer->getUserData() != nullptr)
		{
			delete timer->getUserData();
			timer->setUserData(nullptr);
		}
	};

	timer.start();
}

*/
class Timer;
typedef std::function<void(Timer*)> TimerCallback;

class TimerManager;
class Timer
{
	friend class TimerManager;
public:
	// Create a timer, it will call callback() every dt seconds with callTimes times.
	// If dt == 0, it will be triggered every frame. If callTimes == -1, it will not automatically stop.
	// Call start() to start this timer.
	Timer(float dt, int callTimes, const TimerCallback& callback);
	
	//Create a timer with dt = 0 and callTimes = 0
	Timer();

	virtual ~Timer();

	// Id is the unique symbol of a timer. Some function of TimerManager needs id as param.
	int getId() const;

	// Start timer. In fact it calls TimeManager.addTimer(*this)
	void start();

	// Stop this timer. In fact it calls TimeManager.removeTimer(getId()).
	void stop();

	// Return true if this timer is working.
	bool isWorking() const;

	// Set remain calling times. Timer will automatically call stop() after this count of calling.
	void setRemainCallTimes(int times);

	// Return remain calling times. Timer will automatically call stop() after this count of calling.
	int getRemainCallTimes() const;

	// Timer will call callback every dt time, in seconds. 
	void setDetaTime(float dt);

	// return interval of evrey callback
	float getDetaTime() const;
	
	// Set/Get userData. It's a void pointer that can be set to any thing. 
	void* getUserData() const;
	void setUserData(void* userData);

	// Return how long it has passed since start() be called.
	float getPassedTime() const;
public:
	// set & get callback function
	TimerCallback& callback();
	const TimerCallback& callback() const;

	// set & get onStart funciton. onStart function will be called when you call start()
	TimerCallback& onStart();
	const TimerCallback& onStart()const;

	// set & get onStop funciton. onStop function will be called when you call stop()
	TimerCallback& onStop();
	const TimerCallback& onStop()const;

	// set & get onDestruct function. onDestruct function will be called when Timer destructs.
	TimerCallback& onDestruct();
	const TimerCallback& onDestruct()const;
protected:
	struct TimerInfoSaver
	{
		TimerInfoSaver() :_dt(0), _remainCallTimes(-1), _dtFromLastCall(0), _userData(nullptr), _id(++s_timerid), _workedTime(0) {}
		float _dt;
		int _remainCallTimes;
		float _dtFromLastCall;
		void* _userData;
		float _workedTime;

		TimerCallback _callback;
		TimerCallback _onStart;
		TimerCallback _onStop;
		TimerCallback _onDestruct;
		
		int _id;
		static int s_timerid;
	};
	std::shared_ptr<TimerInfoSaver> _info;
};