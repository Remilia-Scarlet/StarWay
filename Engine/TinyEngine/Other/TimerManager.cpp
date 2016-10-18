#include "TinyEngine/precomp.h"
#include "TinyEngine/Other/TimerManager.h"

bool TimerManager::init()
{
	do 
	{
		return true;
	} while (0);
	return false;
}

TimerManager* TimerManager::s_instance = nullptr;

TimerManager::TimerManager()
	:_enable(true)
{

}

TimerManager::~TimerManager()
{

}

TimerManager* TimerManager::instance()
{
	return s_instance;
}

bool TimerManager::createInstance()
{
	TinyAssert(s_instance == nullptr);
	s_instance = new TimerManager();
	if (!s_instance || !s_instance->init())
		TINY_SAFE_DELETE(s_instance);
	return s_instance != nullptr;
}

void TimerManager::destoryInstance()
{
	TINY_SAFE_DELETE(s_instance);
}

void TimerManager::setEnable(bool enable)
{
	_enable = enable;
}

bool TimerManager::getEnable()
{
	return _enable;
}

void TimerManager::addTimer(const Timer& timer)
{
	auto it = _timers.find(timer.getId());
	if (it == _timers.end())
	{
		Timer& arrTimer = _timers[timer.getId()];
		arrTimer = timer;
		if (timer._info->_onStart)
			timer._info->_onStart(&arrTimer);
	}

}

bool TimerManager::removeTimer(int id)
{
	auto it = _timers.find(id);
	if (it != _timers.end())
	{
		Timer timer = it->second;
		_timers.erase(it);
		if (timer._info->_onStop)
			timer._info->_onStop(&timer);
		return true;
	}
	return false;
}

Timer* TimerManager::getTimer(int id)
{
	auto it = _timers.find(id);
	if (it != _timers.end())
		return &it->second;
	return nullptr;
}

void TimerManager::update(double dt)
{
	if (!_enable)
		return;

	std::list<Timer> callList;
	std::list<Timer> stopList;
	for (auto it = _timers.begin(); it != _timers.end();)
	{
		Timer& timer = it->second;
		Timer::TimerInfoSaver* info = timer._info.get();
		info->_dtFromLastCall += dt;
		if (info->_dtFromLastCall >= info->_dt)
		{
			info->_dtFromLastCall -= info->_dt;
			if ((info->_remainCallTimes > 0 || info->_remainCallTimes == -1) && info->_callback)
				callList.push_back(timer);
			if (info->_remainCallTimes > 0)
			{
				--info->_remainCallTimes;
				++it;
			}
			else if (info->_remainCallTimes == 0)
			{
				if (info->_onStop)
					stopList.push_back(timer);
				it = _timers.erase(it);
			}
			else
				++it;
		}
		else
			++it;
	}
	for (Timer& timer : callList)
	{
		timer._info->_callback(&timer);
	}
	for (Timer& timer : stopList)
	{
		if (timer._info->_onStop)
			timer._info->_onStop(&timer);
	}

}


