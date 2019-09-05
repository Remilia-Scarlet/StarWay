#include "TaskRingBuffer.h"

template <class T, bool RESIZE_ON_FULL>
inline TaskRingBuffer<T, RESIZE_ON_FULL>::TaskRingBuffer(int32_t capacity/* = INITIAL_CAPACITY*/)
	:_dataFlag(static_cast<size_t>(capacity))
{
	_capacity = capacity;
	_data = (T*)new uint8_t[_capacity * sizeof(T)]; //TODO: aligned?
}

template <class T, bool RESIZE_ON_FULL>
inline TaskRingBuffer<T, RESIZE_ON_FULL>::~TaskRingBuffer()
{
	//Set exiting flag and wait till all popping and pushing thread exiting
	setExiting();
	while (_popingThreadNum)
		std::this_thread::yield();
	while (_pushingThreadNum)
		std::this_thread::yield();

	//Destruct all elements
	int end = _back.load();
	int current = _front.load();
	while(current != end)
	{
		_data[current].~T();
		current = (current + 1) % _capacity;
	}
	delete[] (uint8_t*)(_data);
}

template <class T, bool RESIZE_ON_FULL>
void TaskRingBuffer<T, RESIZE_ON_FULL>::pushBack(T elem)
{
	NumGuard pushingThreadNumGuard(_pushingThreadNum);
	if (_isExiting)
		return;

	int32_t back;
	{
		std::unique_lock<std::mutex> lock(_pushMutex);
		back = _back.load();
		if (isFull() || !_dataFlag[back].try_lock())
		{
			if (RESIZE_ON_FULL)
			{
				{
					std::unique_lock<std::mutex> lock(_waitingForPushMtx);
					_blockForFull = true;
				}
				//wait till all popping and pushing threads are waiting or already exiting
				while (!(_workingPopThreadNum.load() == _popingWaitingThreadNum.load()))
					std::this_thread::yield();
				while (_workingPushingThreadNum != 0)
					std::this_thread::yield();
				increaseCapacity();
				{
					std::unique_lock<std::mutex> lock(_waitingForPushMtx);
					_blockForFull = false;
					_waitingForPushCondi.notify_all();
				}
			}
			else
			{
				while (isFull())
					std::this_thread::yield();
			}
			assert(!isFull());
			back = _back.load();
			_dataFlag[back].lock();
		}

		{
			std::unique_lock<std::mutex> lock(_waitingForPushMtx);
			_back = (_back + 1) % _capacity;
		}
		++_workingPushingThreadNum;
	}

	//push back
	new (_data + back)T(std::move(elem));

	_dataFlag[back].unlock();
	_waitingForPushCondi.notify_one();
	--_workingPushingThreadNum;
}

template <class T, bool RESIZE_ON_FULL>
std::optional<T> TaskRingBuffer<T, RESIZE_ON_FULL>::popFront()
{
	NumGuard popingThreadNumGuard(_popingThreadNum);

	if (_isExiting)
		return {};

	int32_t front;
	std::unique_lock<std::mutex> lock(_popMutex,std::defer_lock);
	NumGuard workingPopThreadGuard(_workingPopThreadNum, true);
	{
		lock.lock();
		workingPopThreadGuard.activeGuard();
		while(isEmpty() || _blockForFull.load() || _isExiting)//empty
		{
			NumGuard popingWaitingThreadNumGuard(_popingWaitingThreadNum);
			std::unique_lock<std::mutex> waitingForPushLock(_waitingForPushMtx);
			_waitingForPushCondi.wait(waitingForPushLock, [this]() {return (!isEmpty() || _isExiting) && !_blockForFull; });
			if (_isExiting)
				return {};
		}
		assert(!isEmpty());
		front = _front.load();

		_dataFlag[front].lock();
		_front = (front + 1) % _capacity;
		
	}

	T ret{ std::move(_data[front]) };
	_data[front].~T();

	_dataFlag[front].unlock();
	lock.unlock();
	workingPopThreadGuard.deactiveGuard();

	return std::optional<T>(std::move(ret));
}

template <class T, bool RESIZE_ON_FULL>
inline void TaskRingBuffer<T, RESIZE_ON_FULL>::setExiting()
{
	{
		std::unique_lock<std::mutex> lock(_waitingForPushMtx);
		_isExiting = true;
	}
	_waitingForPushCondi.notify_all();
}

template <class T, bool RESIZE_ON_FULL>
bool TaskRingBuffer<T, RESIZE_ON_FULL>::isFull() const
{
	const int32_t front = _front.load();
	const int32_t back = _back.load();
	return (front <= back ? front + _capacity - back : front - back) <= 1;
}

template <class T, bool RESIZE_ON_FULL>
bool TaskRingBuffer<T, RESIZE_ON_FULL>::isEmpty() const
{
	return _front == _back;
}

template <class T, bool RESIZE_ON_FULL>
inline void TaskRingBuffer<T, RESIZE_ON_FULL>::increaseCapacity()
{
	T* oldData= _data;
	int32_t oldCapacity = _capacity;
	int32_t begin = _front;
	int32_t end = _back;

	//alloc new space
	_capacity = int32_t(_capacity * 1.2);
	_data = (T*)new uint8_t[_capacity * sizeof(T)]; //TODO: aligned?
	std::vector<std::mutex> tmpVec(static_cast<size_t>(_capacity));
	_front = 0;
	_back = 0;

	//move to new space
	while (begin != end)
	{
		int newIndex = _back;
		new (_data + newIndex) T(std::move(oldData[begin]));

		oldData[begin].~T();
		begin = (begin + 1) % oldCapacity;
		++_back;
	}
	delete[] (uint8_t*)(oldData);
	_dataFlag.swap(tmpVec);
}
