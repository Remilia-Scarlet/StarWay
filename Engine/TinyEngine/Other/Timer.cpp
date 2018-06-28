#include "precomp.h"
#include "TinyEngine/Other/Timer.h"
#include "TimerManager.h"

int Timer::TimerInfoSaver::s_timerid = 0;

Timer::Timer(float dt, int callTimes, const TimerCallback& callback)
{
	_info = std::shared_ptr<TimerInfoSaver>(new TimerInfoSaver());
	_info->_dt = dt;
	_info->_remainCallTimes = callTimes;
	_info->_callback = callback;
}

Timer::Timer()
{
	_info = std::shared_ptr<TimerInfoSaver>(new TimerInfoSaver());
}

Timer::~Timer()
{
	if (_info.use_count() == 1)
	{
		if (_info->_onDestruct)
			_info->_onDestruct(this);
	}
}

int Timer::getId() const
{
	return _info->_id;
}

void Timer::start()
{
	TimerManager::instance()->addTimer(*this);
}

void Timer::stop()
{
	TimerManager::instance()->removeTimer(getId());
}

bool Timer::isWorking() const
{
	return _info->_remainCallTimes != 0;
}

void Timer::setRemainCallTimes(int times)
{
	TinyAssert(times >= -1);
	_info->_remainCallTimes = times;
}

int Timer::getRemainCallTimes() const
{
	return _info->_remainCallTimes;
}

void Timer::setDetaTime(float dt)
{
	TinyAssert(dt >= 0);
	if (dt < 0)
		dt = 0;
	_info->_dt = dt;
}

float Timer::getDetaTime() const
{
	return _info->_dt;
}

void* Timer::getUserData() const
{
	return _info->_userData;
}

void Timer::setUserData(void* userData)
{
	_info->_userData = userData;
}

float Timer::getPassedTime() const
{
	return _info->_workedTime;
}

TimerCallback& Timer::callback()
{
	return _info->_callback;
}

const TimerCallback& Timer::callback() const
{
	return _info->_callback;
}

TimerCallback& Timer::onStart()
{
	return _info->_onStart;
}

const TimerCallback& Timer::onStart() const
{
	return _info->_onStart;
}

TimerCallback& Timer::onStop()
{
	return _info->_onStop;
}

const TimerCallback& Timer::onStop() const
{
	return _info->_onStop;
}

TimerCallback& Timer::onDestruct()
{
	return _info->_onDestruct;
}

const TimerCallback& Timer::onDestruct() const
{
	return _info->_onDestruct;
}
