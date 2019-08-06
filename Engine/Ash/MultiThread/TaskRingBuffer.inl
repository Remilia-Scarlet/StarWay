#include "TaskRingBuffer.h"

template<class T>
inline TaskRingBuffer<T>::TaskRingBuffer()
{
	_data = (T*)new uint8_t[INITIAL_CAPACITY * sizeof(T)]; //TODO: aligned?
	_capacity = INITIAL_CAPACITY;
}

template<class T>
inline TaskRingBuffer<T>::~TaskRingBuffer()
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

template<class T>
void TaskRingBuffer<T>::pushBack(T elem)
{
	++_pushingThreadNum;
	TinyAssert(_pushingThreadNum.load() == 1, "You can't call pushBack in two threads at the same time");

	TinyAssert(!_isExiting, "You can't call pushBack when you already called setExiting()");
	if (_isExiting)
		return;

	if(isFull())
	{
		_blockForFull = true;
		//wait till all popping threads are waiting or already exiting
		while (_popingThreadNum != _popingWaitingThreadNum) 
			std::this_thread::yield();
		increaseCapacity();
		_blockForFull = false;
	}
	//push back
	new (_data + _back)T(std::move(elem));
	_back = (_back + 1) % _capacity;
	_waitingForPushCondi.notify_one();
	--_pushingThreadNum;
}

template<class T>
T TaskRingBuffer<T>::popFront()
{
	struct NumGuard
	{
		NumGuard(std::atomic<int32_t>& num) :_num(num) { ++_num; }
		~NumGuard() { --_num; }
		std::atomic<int32_t>& _num;
	};

	NumGuard popingThreadNumGuard(_popingThreadNum);
	TinyAssert(!_isExiting);

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
			//Now currentFront is mine
			T ret{ std::move(_data[currentFront]) };
			_data[currentFront].~T();

			//during the construct time, front has moved forward so much. Don't let it catch up with _back
			//If you assert here, add KEEP_CAPACITY.
			int32_t nowFront = _front;
			TinyAssert((nowFront >= currentFront ? nowFront - currentFront : nowFront + _capacity - currentFront) <= KEEP_CAPACITY);

			return ret;
		}
	}
	return T{};
}

template<class T>
inline void TaskRingBuffer<T>::setExiting()
{
	_isExiting = true;
	_waitingForPushCondi.notify_all();
}

template <class T>
bool TaskRingBuffer<T>::isFull() const
{
	int back = _back.load();
	int front = _front.load();
	return (front <= back ? front + _capacity - back : front - back) <= KEEP_CAPACITY;
}

template <class T>
bool TaskRingBuffer<T>::isEmpty() const
{
	return _front == _back;
}

template<class T>
inline void TaskRingBuffer<T>::increaseCapacity()
{
	T* oldData= _data;
	int32_t oldCapacity = _capacity;
	int32_t begin = _front;
	int32_t end = _back;

	//alloc new space
	_capacity = int32_t(_capacity * 1.2);
	_data = (T*)new uint8_t[_capacity * sizeof(T)]; //TODO: aligned?
	_front = 0;
	_back = 0;

	//move to new space
	while (begin != end)
	{
		new (_data + _back++) T(std::move(oldData[begin]));
		oldData[begin].~T();
		begin = (begin + 1) % oldCapacity;
	}
	delete[] (uint8_t*)(oldData);
}
