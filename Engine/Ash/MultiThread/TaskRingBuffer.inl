#include "TaskRingBuffer.h"

template <class T, bool RESIZE_ON_FULL>
inline TaskRingBuffer<T, RESIZE_ON_FULL>::TaskRingBuffer(int32_t capacity/* = INITIAL_CAPACITY*/)
{
	_capacity = capacity;
	_data = (T*)new uint8_t[_capacity * sizeof(T)]; //TODO: aligned?

	std::vector<std::atomic<Status>> tmpv(_capacity); 
	_dataFlag.swap(tmpv);//unable to use resize and reserve duo to std::atomic uncopyable and unmoveable
	for (auto& data : _dataFlag)
	{
		data = Status::UNINITIALIZED;
	}
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
		std::unique_lock<std::mutex> lock(_resizeMutex);
		back = _back.load();
		if (isFull(_front, back) || _dataFlag[back] != Status::UNINITIALIZED)
		{
			if (RESIZE_ON_FULL)
			{
				_blockForFull = true;
				//wait till all popping and pushing threads are waiting or already exiting
				while (_popingThreadNum != _popingWaitingThreadNum)
					std::this_thread::yield();
				while (_workingPushingThreadNum != 0)
					std::this_thread::yield();
				increaseCapacity();
				_blockForFull = false;
			}
			else
			{
				while (isFull(_front, back) || _dataFlag[back] != Status::UNINITIALIZED)
					std::this_thread::yield();
			}
		}
		back = _back.load();
		TinyAssert(!isFull(_front, back) && _dataFlag[back] == Status::UNINITIALIZED);
		_dataFlag[back] = Status::WRITING;
		_back = (_back + 1) % _capacity;
		++_workingPushingThreadNum;
	}

	//push back
	new (_data + back)T(std::move(elem));

	_dataFlag[back] = Status::FINISH_WRITING;
	_waitingForPushCondi.notify_one();
	--_workingPushingThreadNum;
}

template <class T, bool RESIZE_ON_FULL>
std::optional<T> TaskRingBuffer<T, RESIZE_ON_FULL>::popFront()
{
	NumGuard popingThreadNumGuard(_popingThreadNum);

	while (!_isExiting.load())
	{
		int32_t currentFront = _front.load();
		if (currentFront == _back || _blockForFull.load())//empty or full
		{
			NumGuard popingWaitingThreadNumGuard(_popingWaitingThreadNum);
			std::unique_lock<std::mutex> lock(_waitingForPushMtx);
			_waitingForPushCondi.wait(lock, [this]() {return (!isEmpty() && !_blockForFull) || _isExiting; });
			//while (!((!isEmpty() && !_blockForFull) || _isExiting));
		}
		else
		{
			//CAS popping the front
			if(!_front.compare_exchange_strong(currentFront, (currentFront + 1) % _capacity))
				continue;
			//wait for writing finish
			while (_dataFlag[currentFront] != Status::FINISH_WRITING)
				std::this_thread::yield();
			
			//Now currentFront is mine
			_dataFlag[currentFront] = Status::READING;

			T ret{ std::move(_data[currentFront]) };
			_data[currentFront].~T();

			_dataFlag[currentFront] = Status::UNINITIALIZED;

			return std::optional<T>(std::move(ret));
		}
	}
	return {};
}

template <class T, bool RESIZE_ON_FULL>
inline void TaskRingBuffer<T, RESIZE_ON_FULL>::setExiting()
{
	_isExiting = true;
	_waitingForPushCondi.notify_all();
}

template <class T, bool RESIZE_ON_FULL>
bool TaskRingBuffer<T, RESIZE_ON_FULL>::isFull(int32_t front, int32_t back) const
{
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
	std::vector<std::atomic<Status>> tmpFlag(_capacity);
	for (auto& flag : tmpFlag) 
		flag = Status::UNINITIALIZED; 
	_front = 0;
	_back = 0;

	//move to new space
	while (begin != end)
	{
		int newIndex = _back;
		new (_data + newIndex) T(std::move(oldData[begin]));
		tmpFlag[newIndex] = _dataFlag[begin].load();

		oldData[begin].~T();
		begin = (begin + 1) % oldCapacity;
		++_back;
	}
	delete[] (uint8_t*)(oldData);
	std::swap(_dataFlag, tmpFlag);
}
