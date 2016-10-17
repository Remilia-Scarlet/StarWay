#pragma once
#include <functional>

/*
Usage:

Timer timer = Timer(1,0,somefunction);
timer.start();

Now the somefunction will be called every 1 second.

for example:
void test()
{
	
	std::function<int,int> addFun = [](int x,int y){
		printf("%d\n",x+y);
	}

	Timer timer = Timer(1,10,std::bind(addFun,));
	timer.start();
}

*/


class Timer
{
public:
	// Create a timer, it will call callBack() every dt seconds with callTimes times.
	// If dt == 0, it will be triggered every frame. If callTimes == 0, it will not automatically stop.
	// Call start() to start this timer.
	Timer(float dt, int callTimes, const std::function<void>& callBack, ... );
	
	~Timer();

	// Start timer
	void start();

	// Stop this timer
	void stop();

	// Return true if this timer is working.
	void isWorking() const;

	// Set remain calling times. Timer will automatically call stop() after this count of calling.
	void setRemainCallTimes(int times);

	// Return remain calling times. Timer will automatically call stop() after this count of calling.
	int getRemainCallTimes() const;

	// Timer will call callback every dt time, in seconds. 
	void setDetaTime(float dt);

	float getDetaTime() const;

	// Set call back fucntion

	const std::function<void>& 
};